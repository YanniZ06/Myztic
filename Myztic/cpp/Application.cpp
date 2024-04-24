#include "pch.h"
#include "framework.h"

#include <Application.h>
#include <Scene.h>

#include <SDL.h>
#include <glad.h>

#include <string>
#include <iostream>
#include <ErrorHandler.hpp>

#include <mutex>

#define SDL_MAIN_HANDLED

std::map<unsigned char, std::shared_ptr<Window>> Application::windows;
// std::thread mainThread;
bool Application::shouldClose = false;

void Application::initMyztic() {
	SDL_SetMainReady();

	// mainThread = std::thread(std::this_thread::get_id());

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0) {
		throw "Error initializing SDL subsystems : " + std::string(SDL_GetError());
	}

	WindowParams params = { "Myztic Engine Test", new Scene(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
	Window* window = Window::create(params);

	// Segunda Windowa (excellente espanol)
	WindowParams paramsB = { "Myztic Engine Test 2", new Scene(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
	Window* windowB = Window::create(paramsB);
	log_windows_cmd();
	windowB->setX(windowB->x() + 250);

	SDL_GL_MakeCurrent(window->handle, window->context);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	CHECK_GL(glViewport(0, 0, 680, 480));
}

void Application::app_loop() {
	while (!shouldClose) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) shouldClose = true;
		}

		SDL_Delay(1);
	}
}

void Application::start_winloop(Window* win) {
	// TODO: Mutex, wait for next event rotation thingamajig lols hahahahahaha amoignus
	window_loop(win);
}

// Todo: Run soely physics and drawing logic in here, (preloading is done on ANOTHER thread if we do the context preloading thing)
// Event handling should be done on the main thread
void Application::window_loop(Window* win) {
	//while (!win->shouldClose) {

		SDL_Delay(2000); // just wait 2 seconds lol
	//}
}

// Logging and shit

void Application::log_windows_cmd() {
	std::cout << "Application::windows =>\n";
	for (std::map<unsigned char, std::shared_ptr<Window>>::const_iterator it = Application::windows.begin();
		it != Application::windows.end(); ++it)
	{
		std::cout << (int)it->first << " -> " << (std::string)*it->second.get() << "\n";
	}
}