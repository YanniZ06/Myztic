#pragma once

#include <map>
#include <memory>

#include "display/Window.h"

// The core of Myztic. Contains all sorts of useful fields and initializes the engine.
class Application {
public:
    // Current windows, mapped to their ID
    static std::map<unsigned char, std::shared_ptr<Window>> windows;

    // Initialize Myztic Engine
    static void initMyztic();
};