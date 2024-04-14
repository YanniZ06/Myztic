#include "pch.h"
#include "include/Application.h"

#include "SDL.h"
#include <string>
#include <iostream>

// Initialize Myztic Engine
void Application::initMyztic() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0) {
        throw "Error initializing SDL subsystems : " + std::string(SDL_GetError());
    }
}

//void newWindow();