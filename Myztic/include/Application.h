#pragma once

#include <map>
#include <memory>
#include <thread>

#include <display/Window.h>

#define MYZTIC_WIN_LIMIT 15

// The core of Myztic. Contains all sorts of useful fields and initializes the engine.
class Application {
    friend class Window;
private: 
    static void app_loop();
public:
    // Current windows, mapped to their ID
    static std::map<unsigned char, std::shared_ptr<Window>> windows;
    // All running threads, mapped to their task/use-case
    static std::map<const char*, std::shared_ptr<std::thread>> threads;
    //static std::thread mainThread;

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