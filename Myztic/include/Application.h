#pragma once

#include <map>
#include <memory>
#include <thread>
#include <chrono>
//#include <semaphore>

#include <display/Fps.h>
#include <utilities/thread/ResourceManager.h>

#define MYZTIC_WIN_LIMIT 16 // Real limit should be 256, but we use this instead because nobody should need that many windows. Might be tweaked later

namespace Myztic {
    enum class VSyncType : int8_t {
        VSYNC_ADAPATIVE = -1,
        VSYNC_NONE = 0,
        VSYNC_ON = 1
    };

    class Window;
    struct WindowParams;
    /**
     * The core of Myztic. Contains all sorts of useful fields and initializes the engine.
     */
    class Application {
        friend class Window;
        friend class WinThread;
    private:
        /// Amount of WinThreads that are done with their update loop
        static std::atomic<uint8_t> readyWinThreads;
        /// Different to the windows map size, this is only updated when a window has been fed into the loop 
        static uint8_t registeredWinThreads;

        static void app_loop();
    public:
        /// Current windows, mapped to their ID
        static std::map<unsigned int, Window*> windows;
        /// All running threads, mapped to their task/use-case
        static std::map<const char*, std::thread*> threads;

        /// Dictates whether the main thread resources are currently available or being read/modified by one of the threads
        static ResourceManager* resourceManager;

        /// Used to allow drawing operations from any custom thread without the danger of invalidating a concurrent rendering process.
        /// 
        /// If the main thread is currently drawing, a request will halt until the draw phase is over, and like-wise a draw phase will be prevented until your request was finished.
        static ResourceManager threadSafeDraw; // todo: once threads and resource managers are more figured out, following this commits' todos and etc

        /// Tells the main loop thread to wait on the other window threads
        static std::binary_semaphore* waiter;

        /// The fps settings for this application
        static Fps fps;
        static bool shouldClose;

        // todo: wip
        /**
         * Sets the VSync option for this Application.
         * 
         * \remark Depending on OS or hardware options, the setting might not be applied.
         */
        /*
        static inline void vSync(VSyncType setting) {
            for (auto it = windows.begin(); it != windows.end(); ++it) {
                it->second->switchToContext();
            }
        }
        */

        /**
         * Initializes Myztic Engine.
         *
         * \param initWindowParams Parameters for the main window, check the WindowParams type for more info.
         * \param fps The max FPS the application should run at, can be configured afterwards.
         * \throw MYZTIC_INIT_SDL_ERROR along with error information if one of the following SDL subsystems could not be initialized:
         * \n `SDL_INIT_VIDEO`, `SDL_INIT_EVENTS`, `SDL_INIT_GAMECONTROLLER`, `SDL_INIT_JOYSTICK` or `SDL_INIT_HAPTIC`
         */
        static void initMyztic(WindowParams& initWindowParams, fpsSize fps);
        // Logs the window map to the command line
        static void log_windows_cmd();

    protected:
        static VSyncType vSync_; // todo: wip

        // Starts an update loop for the input window, but makes sure to wait for the next event queue
        static void start_winloop(Window* win);
        // Update loop execution for the window
        static void window_loop(Window* win);
    };
}