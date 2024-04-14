#pragma once

#include <map>
#include "include/display/Window.h"

class Application {
public:
    // Current windows, mapped to their ID
    static std::map<unsigned char, Window> windows;

    // Initialize Myztic Engine
    static void initMyztic();

    //void newWindow();
};