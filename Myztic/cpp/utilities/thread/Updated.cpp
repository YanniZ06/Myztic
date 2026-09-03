#include <pch.h>

#include <utilities\thread\ResourceManager.h>
#include <utilities/Logger.h>


using namespace Myztic;

ResourceManager::ResourceManager() {
    if (!hasStaticInstance.load()) {
        threadMapWaiter = new std::binary_semaphore(1);
        hasStaticInstance.store(true);
    }
}


bool ResourceManager::request(bool keepOwnedResources) {
    std::thread::id caller_threadId = std::this_thread::get_id();

    ResourceManager::threadMapWaiter->acquire();
    if (active && current_threadId == caller_threadId) {
        ResourceManager::threadMapWaiter->release();

        Logger::logWarning("Attempted to request same resource twice on the same thread!", 1);
        return true;
    }
    else {
        if (waiter.try_acquire()) {
            current_threadId = caller_threadId;
            ResourceManager::threadResources[current_threadId].push_back(this); // If the resource was free it is no longer marked as such
            ResourceManager::threadMapWaiter->release();
            return active = true;
        }
        else {
            // Free all other current managers tied to this thread until the caller thread activates again, since theyre available and could also cause deadlocking a different thread along with this one
            std::vector<ResourceManager*>& managerList = ResourceManager::threadResources[caller_threadId];

            if (!keepOwnedResources) {
                for (ResourceManager* manager : managerList) {
                    // if (manager->active) {
                    manager->waiter.release();
                    manager->active = false;
                    // }
                }
            }
            managerList.push_back(this);
            ResourceManager::threadMapWaiter->release();

            waiter.acquire(); // Make sure we actually acquire anyways so the thread is locked (or unlocked if by now the resource is already free again)
            active = true;

            // Retry indefinetly (todo: add stop after x seconds)
            while (!keepOwnedResources) {
                ResourceManager* failedManager = nullptr;

                ResourceManager::threadMapWaiter->acquire();
                for (ResourceManager* manager : managerList) {
                    if (manager->active) continue; // Skip the manager we just gained access to since there is always one

                    if (!manager->retryOccupy()) {
                        failedManager = manager;
                        break;
                    }
                }

                if (failedManager) {
                    for (ResourceManager* manager : managerList) {
                        if (manager->active) {
                            manager->waiter.release();
                            manager->active = false;
                        }
                    }
                    ResourceManager::threadMapWaiter->release();

                    failedManager->waiter.acquire(); // Lock again until free
                    failedManager->active = true;
                }
                else {
                    break;
                }
            }


            return false;
        }
    }
    // return true;
}

void ResourceManager::finishRequest(bool forceRelease) {
    if (forceRelease) {
        waiter.release();
        active = false;
    }

    std::vector<ResourceManager*>& managers = ResourceManager::threadResources[current_threadId];
    if (managers.size() > 0) {
        try {
            managers.erase(find(managers.begin(), managers.end(), this));
            if (!forceRelease) {
                waiter.release();
                active = false;
            }
        }
        catch (std::exception e) {
            std::string msg = "Attempted request finish on thread where no request was made (";
            msg.append(e.what());
            msg.append(")");

            Logger::logWarning(msg, 1);
        }
    }
    else {
        std::string msg = "Attempted request finish on thread where no request was made";
        Logger::logWarning(msg, 1);
    }
}

std::binary_semaphore* ResourceManager::threadMapWaiter;
std::map<std::thread::id, std::vector<ResourceManager*>> ResourceManager::threadResources;
std::atomic_bool ResourceManager::hasStaticInstance;
/*
std::map<std::thread::id, std::vector<ResourceManager*>> threadResources;
std::map<std::thread::id, std::vector<std::condition_variable*>> waitingThreads;
std::mutex globalMutex;
*/