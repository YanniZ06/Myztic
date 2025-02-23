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

        // todo: type specified resource manager, as in where an accessor may access a resource that is already used by a different accessor, but is allowed to use it anyways because they 
        // todo: share the same accessor type and dont cause any conflicts
        // inline bool queueRequest() {}

        // todo: #2: create a system that keeps track of which threads hold which resource-manager, so that a situation in which two threads which first aquired different managers
        // todo: lock eachother out like this: (t2->draw ; t1->general; t1->draw [T1 HALTS], t2->general [T2 HALTS];) doesnt occur
        // todo: INSTEAD, if a thread is halted it should automatically free up all other resource managers it is holding onto, put them in a queue, and once theyre unhalted, hold onto their original resources
        // todo: ONLY if they are not currently occupied, and if they are, wait until they no longer are (basically be forced to wait at the back of the line)

        /**
         * Notifies the resource manager that the last thread has finished using the resource so that the next one in queue can start accessing it.
         * \warning This function should only be called if `request()` was called before, otherwise it will invalidate requests sent by other threads and cause otherwise undefined behavior.
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