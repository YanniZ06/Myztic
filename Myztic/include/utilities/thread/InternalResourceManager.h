#pragma once

#include <SDL_stdinc.h>
#include <semaphore>

namespace Myztic {
    class InternalResourceManager {
    public:
        InternalResourceManager() = default;

        void request() {
            waiter.acquire();
        }

        void finishRequest() {
            waiter.release();
        }

    private:
        std::binary_semaphore waiter{ 1 };
    };

}
