#pragma once

#include <SDL_stdinc.h>
#include <semaphore>
#include <map>
#include <thread>
#include <vector>

#include <utilities/Logger.h>

namespace Myztic {

    class ResourceManager {
    public:
        ResourceManager();

        /**
        * Requests access to a threaded resource, blocks the calling thread if another one is already using the resource and releases once it is finished.
        * \n Threads are put into a queue based on when they requested access.
        * \n
        * \n By default, any other ResourceManagers currently owned by the calling thread will be freed incase the calling thread cannot access this ResourceManager right now.
        * \n This behaviour can be turned off with the argument `keepOwnedresources
        * @param keepOwnedResources If true, all ResourceManagers owned by the calling thread will stay occupied, even if it blocks due to this ResourceManager already being occupied.
        * \note Make sure to keep track of- and avoid two threads locking eachother out when keepOwnedResources is set to true.
        * \warn Every call to this function must be followed by a subsequent `finishRequest()` call once the resource is no longer in use by the calling thread.
        * \see finishRequest
        *
        * \return True if the request was handled immediately, false if the resource was already in use.
        */
        bool request(bool keepOwnedResources = false) {
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
                    std::vector<ResourceManager*>& managerList = ResourceManager::threadResources[current_threadId];

                    for (ResourceManager* manager : managerList) {
                        // if (manager->active) {
                        manager->waiter.release();
                        manager->active = false;
                        // }
                    }
                    managerList.push_back(this);
                    ResourceManager::threadMapWaiter->release();

                    waiter.acquire(); // Make sure we actually acquire anyways so the thread is locked (or unlocked if by now the resource is already free again)
                    active = true;

                    // Retry indefinetly (todo: add stop after x seconds)
                    while (true) {
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

        // Release access
        /**
         * Notifies the resource manager that the last thread has finished using the resource so that the next one in queue can start accessing it.
         * @param forceRelease If true, forces the resource to be released and made accessible for the next thread even if this thread did not currently own the resource.
         * \warning This function should only be called if `request()` was called before, otherwise it could possibly invalidate requests sent by other threads and cause otherwise undefined behavior.
         * \see request
         */

        void finishRequest(bool forceRelease = false) {
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


    private:
        std::binary_semaphore waiter{ 1 };

        std::thread::id current_threadId;

        bool active = false; // Only used internally for checking threadResources

        static std::binary_semaphore* threadMapWaiter;
        static std::map<std::thread::id, std::vector<ResourceManager*>> threadResources;
        static std::atomic_bool hasStaticInstance;

        bool retryOccupy() {
            return active = waiter.try_acquire();
        }

        /*
        void finishRequest_Temp() {
            waiter.release();
        }
        */
    };
}
