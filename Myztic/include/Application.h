#pragma once

#include <map>
#include <memory>
#include <thread>
#include <chrono>
//#include <semaphore>

#include <display/Window.h>
#include <display/Fps.h>

#define MYZTIC_WIN_LIMIT 16 // Real limit is 256, but we use this instead because nobody should need that many windows. Might be tweaked later

// The core of Myztic. Contains all sorts of useful fields and initializes the engine.
class Application {
    friend class Window;
    friend class WinThread;
private: 
    static unsigned char readyWinThreads; // Amount of WinThreads that are done with their update loop
    static unsigned char registeredWinThreads; // Different to the windows map size, this is only updated when a window has been fed into the loop 

    static void app_loop();
    static void _initMyztic(WindowParams p, fpsSize fps);
public:
    // Current windows, mapped to their ID
    static std::map<unsigned char, std::shared_ptr<Window>> windows;
    // All running threads, mapped to their task/use-case
    static std::map<const char*, std::shared_ptr<std::thread>> threads;


    // The main myztic thread
    static std::thread mainThread;
    // Dictates whether the main thread resources are currently available or being read/modified by one of the threads
    static std::binary_semaphore* resourceManager;
    // Tells the main loop thread to wait on the other window threads
    static std::binary_semaphore* waiter;

    // Fps settings for this application
    static Fps fps;
    static bool shouldClose;


    // Initializes Myztic Engine
    static void initMyztic(WindowParams initWindowParams, fpsSize fps);
    // Logs the window map to the command line
    static void log_windows_cmd();

protected:
    // Starts an update loop for the input window, but makes sure to wait for the next event queue
    static void start_winloop(Window* win);
    // Update loop execution for the window
    static void window_loop(Window* win);
};