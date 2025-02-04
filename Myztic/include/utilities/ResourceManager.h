#pragma once

#include <SDL_stdinc.h>
#include <semaphore>

/**
 * Class used to handle resource access over multiple threads in safe manner, avoiding race conditions.
 */
namespace Myztic {
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
            if (busy.load()) {
                waiter->acquire();
                return false;
            }
            busy.store(true);
            return true;
        }

        /**
         * Notifies the resource manager that the last thread has finished using the resource so that the next one in queue can start accessing it.
         * \warning This function should only be called if `request()` was called before, otherwise it will invalidate requests sent by other threads.
         * \see request
         *
         * \return True if there was a request to finish, false if there were no requests.
         */
        inline bool finishRequest() {
            if (!busy.load()) return false;

            requests--;
            if (requests.load() == 0) busy.store(false);
            waiter->release();
            return true;
        }

        /**
         * Returns whether the resource manager is currently handling a resource or not.
         */
        inline bool isBusy() {
            return busy.load();
        }
    protected:
        std::atomic<uint8_t> requests;
        std::atomic<bool> busy;
        std::binary_semaphore* waiter;
    };
}