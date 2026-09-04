#pragma once

#include <SDL_stdinc.h>
#include <semaphore>
#include <map>
#include <thread>
#include <vector>

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
        bool request(bool keepOwnedResources = false);

        // Release access
        /**
         * Notifies the resource manager that the last thread has finished using the resource so that the next one in queue can start accessing it.
         * @param forceRelease If true, forces the resource to be released and made accessible for the next thread even if this thread did not currently own the resource.
         * \warning This function should only be called if `request()` was called before, otherwise it could possibly invalidate requests sent by other threads and cause otherwise undefined behavior.
         * \see request
         */
        void finishRequest(bool forceRelease = false);


    private:
        std::binary_semaphore waiter{ 1 };

        // Warning, only valid when active = true, the old owner_threadId is never set to a null value!!
        std::thread::id owner_threadId;

        bool active = false; // Only used internally for checking threadResources

        static std::binary_semaphore* threadMapWaiter;
        static std::map<std::thread::id, std::vector<ResourceManager*>> threadResources;
        static std::atomic_bool hasStaticInstance;

        bool retryOccupy(std::thread::id caller_threadId);

        /*
        void finishRequest_Temp() {
            waiter.release();
        }
        */
    };
}
