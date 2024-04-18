#include "pch.h"
#include "framework.h"

#include <Application.h>
#include <Scene.h>

#include <SDL.h>
#include <glad.h>

#include <string>
#include <iostream>
#include <ErrorHandler.hpp>

#define SDL_MAIN_HANDLED

std::map<unsigned char, std::shared_ptr<Window>> Application::windows;

// Initialize Myztic Engine
void Application::initMyztic() {
	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0) {
		throw "Error initializing SDL subsystems : " + std::string(SDL_GetError());
	}

	WindowParams params = { "Myztic Engine Test", new Scene(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
	Window* window = Window::create(params);

	// Segunda Windowa (excellente espanol)
	WindowParams paramsB = { "Myztic Engine Test 2", new Scene(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
	Window* windowB = Window::create(paramsB);
	windowB->setX(windowB->x() - 250);

	SDL_GL_MakeCurrent(window->handle, window->context);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	log_windows_cmd();

	CHECK_GL(glViewport(0, 0, 680, 480));
}

void Application::log_windows_cmd() {
	std::cout << "Application::windows =>\n";
	for (std::map<unsigned char, std::shared_ptr<Window>>::const_iterator it = Application::windows.begin();
		it != Application::windows.end(); ++it)
	{
		std::cout << (int)it->first << " -> " << (std::string)*it->second.get() << "\n";
	}
}

//void newWindow();