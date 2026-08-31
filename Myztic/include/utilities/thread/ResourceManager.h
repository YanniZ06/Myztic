#pragma once

#include <mutex>
#include <algorithm>
#include <condition_variable>

#include <SDL_stdinc.h>
#include <semaphore>
#include <map>
#include <thread>
#include <vector>

#include <utilities/thread/InternalResourceManager.h>

namespace Myztic {

    class ResourceManager {
    public:
        ResourceManager();

        // Request access to this resource
         /**
         * Requests access to a threaded resource, halts the calling thread if another one is already using the resource and releases once it is finished.
         * \n Threads are put into a queue based on when they requested access.
         * \n Every call to this function should be followed by a subsequent `finishRequest()` call once the resource is no longer in use by the calling thread.
         * \warning A thread is only allowed to call request once per resource manager.\n If it calls twice the requesting thread will freeze until a different one calls `finishRequest()`.
         * \see finishRequest
         *
         * \return True if the request was handled immediately, false if the resource is already in use.
         */
        bool request() {
            current_threadId = std::this_thread::get_id();

            // If its not busy this statement will be skipped and the request is granted, otherwise run the logic
            if (!waiter.try_acquire()) 
            {
                ResourceManager::threadMapWaiter->acquire();

                // If there are no other managers to worry about, start holding right away
                if (!ResourceManager::threadResources.contains(current_threadId) || ResourceManager::threadResources[current_threadId].size() == 0) 
                {
                    ResourceManager::threadResources[current_threadId].push_back(this);
                    ResourceManager::threadMapWaiter->release();
                    
                    waiter.acquire();
                    return false;
                }
                // Free all other current managers tied to this thread until the caller thread activates again, since theyre available and could also cause deadlocking a different thread along with this one
                std::vector<ResourceManager*> haltedManagersList = ResourceManager::threadResources[current_threadId]; // Get a copy of current managers 
                std::vector<ResourceManager*>& managerList = ResourceManager::threadResources[current_threadId];
                auto it = managerList.begin();
                for (ResourceManager* manager : managerList) {
                    manager->finishRequest_Temp();
                    managerList.erase(it++);
                }

                ResourceManager::threadResources[current_threadId].push_back(this);

                ResourceManager::threadMapWaiter->release();

                waiter.acquire(); // Make sure we actually acquire anyways so the thread is locked
                return false;
            }
            return true;
        }

        // Release access
        /**
         * Notifies the resource manager that the last thread has finished using the resource so that the next one in queue can start accessing it.
         * \warning This function should only be called if `request()` was called before, otherwise it will invalidate requests sent by other threads and cause otherwise undefined behavior.
         * \see request
         */

        void finishRequest() {
            waiter.release();
            //vect
            std::vector<ResourceManager*>& managers = ResourceManager::threadResources[current_threadId];
            if (managers.size() > 0) {
                managers.erase(find(managers.begin(), managers.end(), this));
            }
        }


    private:
        std::binary_semaphore waiter{ 1 };
        
        std::thread::id current_threadId;

        static std::binary_semaphore* threadMapWaiter;
        static std::map<std::thread::id, std::vector<ResourceManager*>> threadResources;
        static std::atomic_bool hasStaticInstance;

        void finishRequest_Temp() {
            waiter.release();
        }
    };

// todo: #2: create a system that keeps track of which threads hold which resource-manager, so that a situation in which two threads which first aquired different managers
// todo: lock eachother out like this: (t2->draw ; t1->general; t1->draw [T1 HALTS], t2->general [T2 HALTS];) doesnt occur
// todo: INSTEAD, if a thread is halted it should automatically free up all other resource managers it is holding onto, put them in a queue, and once theyre unhalted, hold onto their original resources
// todo: ONLY if they are not currently occupied, and if they are, wait until they no longer are (basically be forced to wait at the back of the line)

}

// todo: type specified resource manager, as in where an accessor may access a resource that is already used by a different accessor, but is allowed to use it anyways because they 
// todo: share the same accessor type and dont cause any conflicts
// inline bool queueRequest() {}
