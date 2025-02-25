#pragma once

#include <SDL_stdinc.h>
#include <semaphore>
#include <map>
#include <thread>
#include <vector>

#include <utilities/thread/InternalResourceManager.h>

namespace Myztic {
    /**
     * Class used to handle resource access over multiple threads in safe manner, avoiding race conditions.
     * 
     * \note If you wish to create and use your own resource manager, you should allocate it on the heap (whether that is done over a shared pointer or a direct call to "new" is irrelevant).
     */
    class ResourceManager {
    public:
        ResourceManager();
        // ResourceManager() = default;

        /**
         * Requests access to a threaded resource, halts the calling thread if another one is already using the resource and releases once it is finished.
         * \n Threads are put into a queue based on when they requested access.
         * \n Every call to this function should be followed by a subsequent `finishRequest()` call once the resource is no longer in use by the calling thread.
         * \warning A thread is only allowed to call request once per resource manager.\n If it calls twice the requesting thread will freeze until a different one calls `finishRequest()`.
         * \see finishRequest
         *
         * \return True if the request was handled immediately, false if the resource is already in use.
         */
        inline bool request() {
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

        // todo: #2: create a system that keeps track of which threads hold which resource-manager, so that a situation in which two threads which first aquired different managers
        // todo: lock eachother out like this: (t2->draw ; t1->general; t1->draw [T1 HALTS], t2->general [T2 HALTS];) doesnt occur
        // todo: INSTEAD, if a thread is halted it should automatically free up all other resource managers it is holding onto, put them in a queue, and once theyre unhalted, hold onto their original resources
        // todo: ONLY if they are not currently occupied, and if they are, wait until they no longer are (basically be forced to wait at the back of the line)

        /**
         * Notifies the resource manager that the last thread has finished using the resource so that the next one in queue can start accessing it.
         * \warning This function should only be called if `request()` was called before, otherwise it will invalidate requests sent by other threads and cause otherwise undefined behavior.
         * \see request
         */
        inline void finishRequest() {
            if (!busy.load()) return; // comment out??

            requests--;
            if (requests.load() == 0) busy.store(false);

            waiter->release(); // We can already release here, as the rest management will not be able to cause any race conditions of its own 

            std::thread::id threadId = std::this_thread::get_id();

            mapManager.request();
            std::vector<ResourceManager*>& managerList = ResourceManager::thread_Managers[threadId];
            managerList.erase(std::find(managerList.begin(), managerList.end(), this)); // Resource Manager is no longer used by this thread, stop keeping track of it

            if (ResourceManager::thread_Managers[threadId].size() == 0) ResourceManager::thread_Managers.erase(threadId); // If there are no other managers in use for the thread, empty map entry
            mapManager.finishRequest();
        }

        /**
         * Returns whether the resource manager is currently handling a resource or not.
         */
        inline bool isBusy() {
            return busy.load();
        }
    protected:
        static std::map<std::thread::id, std::vector<ResourceManager*>> thread_Managers; // todo: in cpp-file
        static InternalResourceManager mapManager; // todo: in cpp-file

        // Requests the resource whilst keeping track of the other managers that are waiting 
        inline void _requestHeld(std::vector<ResourceManager*>& heldManagers) {
            requests++;
            std::thread::id threadId = std::this_thread::get_id();
            if (busy.load()) {

                // If this is the only other thread paused, we can just wait it out // ERRRRR our original manager is now back in the actual managerList !!
                /*if (heldManagers.size() < 2) {
                    waiter->acquire();

                    mapManager.request();
                    ResourceManager::thread_Managers[threadId].push_back(this);
                    mapManager.finishRequest();

                    return;
                }*/

                // Free all managers that were accessible but would now be wasted again due to another forced waiting period
                mapManager.request();

                std::vector<ResourceManager*>& managerList = ResourceManager::thread_Managers[threadId]; // This only contains each manager that successfully requested since the halt
                for (ResourceManager* manager : managerList) {
                    manager->_finishRequestHold();
                }
                managerList.erase(managerList.begin(), managerList.end()); // Empty the active "real" queue so we can properly use it with regular requests later
                managerList.push_back(this);  // By the time the waiter has finished this is already an actively resource-locking manager

                mapManager.finishRequest();

                //! We start actually waiting here
                waiter->acquire();
                // Our thread is active again, attempt requesting

                std::vector<ResourceManager*> toReset = heldManagers;
                // If we halt we need to take care of all managers that need to be running since the manager isnt held anymore we wont request it for the upcoming-loop though
                heldManagers.erase(std::find(heldManagers.begin(), heldManagers.end(), this));

                for (ResourceManager* h_manager : heldManagers) {
                    h_manager->_requestHeld(toReset); // Cycles until all resource managers are available at once
                }

                return;
            }
            busy.store(true);

            mapManager.request();
            ResourceManager::thread_Managers[threadId].push_back(this);
            mapManager.finishRequest();
        }

        // Frees the manager from the thread quicker, used for halting requests temporarily during thread sleeping/waiting (to allow resources to not be wasted)
        inline void _finishRequestHold() {
            requests--;
            if (requests.load() == 0) busy.store(false);

            waiter->release(); 
        }

        std::atomic<uint8_t> requests;
        std::atomic<bool> busy;
        std::binary_semaphore* waiter;
    };
}

// todo: type specified resource manager, as in where an accessor may access a resource that is already used by a different accessor, but is allowed to use it anyways because they 
// todo: share the same accessor type and dont cause any conflicts
// inline bool queueRequest() {}


// only if normal erase on finishRequest starts tweaking
/*auto rawManagerPosition = std::find(managerList.begin(), managerList.end(), this);
auto realPosition = rawManagerPosition - managerList.begin();
std::vector<ResourceManager*>::const_iterator posIterator = managerList.begin() + realPosition;*/
