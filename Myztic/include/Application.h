#pragma once

#include <map>
#include <memory>
#include <thread>
#include <chrono>
#include <semaphore>

#include <display/Window.h>

#define MYZTIC_WIN_LIMIT 15

// The core of Myztic. Contains all sorts of useful fields and initializes the engine.
class Application {
    friend class Window;
private: 
    static void app_loop();
    static void _initMyztic();
public:
    // Current windows, mapped to their ID
    static std::map<unsigned char, std::shared_ptr<Window>> windows;
    // All running threads, mapped to their task/use-case
    static std::map<const char*, std::shared_ptr<std::thread>> threads;
    // Signals to tell threads when to start and stop execution
    static std::map<const char*, std::shared_ptr<std::binary_semaphore>> tSignals;
    // std::binary_semaphore smphSignalThreadToMain{ 0 };

    // The main myztic thread
    static std::thread mainThread;

    static bool shouldClose;


    // Initializes Myztic Engine
    static void initMyztic();
    // Logs the window map to the command line
    static void log_windows_cmd();

protected:
    // Starts an update loop for the input window, but makes sure to wait for the next event queue
    static void start_winloop(Window* win);
    // Update loop execution for the window
    static void window_loop(Window* win);
};