#pragma once

#include <SDL_stdinc.h>
#include <semaphore>

/// Lighter Resource Manager type used for ResourceManagers themselves
class InternalResourceManager {
public:
    InternalResourceManager();
    // ResourceManager() = default;

    /**
     * Requests access to a threaded resource, halts the calling thread if another one is already using the resource and releases once it is finished.
     * \n Threads are put into a queue based on when they requested access.
     * \n Every call to this function should be followed by a subsequent `finishRequest()` call once the resource is no longer in use by the calling thread.
     * \warning A thread is only allowed to call request once per resource manager.\n If it calls twice the requesting thread will freeze until a different one calls `finishRequest()`.
     * \see finishRequest
     */
    inline void request() {
        requests++;
        if (busy.load()) {
            waiter->acquire();
            return;
        }
        busy.store(true);
    }

    /**
     * Notifies the resource manager that the last thread has finished using the resource so that the next one in queue can start accessing it.
     * \warning This function should only be called if `request()` was called before, otherwise it will invalidate requests sent by other threads and cause otherwise undefined behavior.
     * \see request
     */
    inline void finishRequest() {
        requests--;
        if (requests.load() == 0) busy.store(false);
        waiter->release();
    }
protected:
    std::atomic<uint8_t> requests;
    std::atomic<bool> busy;
    std::binary_semaphore* waiter;
};