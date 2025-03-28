#include <pch.h>
#include <utilities\thread\ResourceManager.h>

using namespace Myztic;

std::map<std::thread::id, std::vector<ResourceManager*>> ResourceManager::thread_Managers;
InternalResourceManager ResourceManager::mapManager;

InternalResourceManager::InternalResourceManager() {
    busy.store(false);
    waiter = new std::binary_semaphore(0);
    requests.store(0);
};

ResourceManager::ResourceManager() {
    busy.store(false);
    waiter = new std::binary_semaphore(0);
    requests.store(0);
}

bool ResourceManager::request() 
{
    requests++;
    std::thread::id threadId = std::this_thread::get_id();
    if (busy.load()) {

        // If there are no other managers to worry about, start holding right away
        if (!ResourceManager::thread_Managers.contains(threadId) || ResourceManager::thread_Managers[threadId].size() == 0) {
            waiter->acquire();

            mapManager.request();
            ResourceManager::thread_Managers[threadId].push_back(this);
            mapManager.finishRequest();

            return false;
        }

        // Free all other current managers tied to this thread until the caller thread activates again, since theyre available and could also cause deadlocking a different thread along with this one
        mapManager.request();
        std::vector<ResourceManager*> haltedManagersList = ResourceManager::thread_Managers[threadId]; // Get a copy of current managers 
        std::vector<ResourceManager*>& managerList = ResourceManager::thread_Managers[threadId];
        for (ResourceManager* manager : managerList) {
            manager->_finishRequestHold();
        }
        managerList.erase(managerList.begin(), managerList.end()); // Empty the active "real" queue so we can properly use it with regular requests later
        managerList.push_back(this); // By the time the waiter has finished this is already an actively resource-locking manager
        mapManager.finishRequest();

        //! We start actually waiting here
        waiter->acquire();
        // Our thread is active again, attempt requesting

        std::vector<ResourceManager*> toReset = haltedManagersList;
        toReset.push_back(this); // If we halt again we want to also request this manager again, we just dont want to request it right now because it is already accessible to this thread
        for (ResourceManager* h_manager : haltedManagersList) {
            h_manager->_requestHeld(toReset);
        }

        return false;
    }
    busy.store(true);
    // 
    mapManager.request();
    ResourceManager::thread_Managers[threadId].push_back(this);
    mapManager.finishRequest();

    return true;
}

void ResourceManager::finishRequest()
{
    if (!busy.load()) return; // comment out??

    requests--;
    if (requests.load() == 0) busy.store(false);

    waiter->release(); // We can already release here, as the rest management will not be able to cause any race conditions of its own 
    waiter->acquire();

    std::thread::id threadId = std::this_thread::get_id();

    mapManager.request();
    std::vector<ResourceManager*>& managerList = ResourceManager::thread_Managers[threadId];
    managerList.erase(std::find(managerList.begin(), managerList.end(), this)); // Resource Manager is no longer used by this thread, stop keeping track of it

    if (ResourceManager::thread_Managers[threadId].size() == 0) ResourceManager::thread_Managers.erase(threadId); // If there are no other managers in use for the thread, empty map entry
    mapManager.finishRequest();
}