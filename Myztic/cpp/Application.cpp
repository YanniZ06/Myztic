#include "pch.h"
#include "framework.h"

#include <Application.h>
#include <Scene.h>

#include <SDL.h>
#include <glad.h>

#include <Timer.h>
#include <ErrorHandler.hpp>

#define SDL_MAIN_HANDLED

std::map<unsigned char, std::shared_ptr<Window>> Application::windows;
std::thread Application::mainThread;
bool Application::shouldClose = false;

void Application::initMyztic(Scene* initScene) {
	mainThread = std::thread(_initMyztic, initScene);
}

void Application::_initMyztic(Scene* initScene) {
	double myzStart = Timer::stamp();
	SDL_SetMainReady();

	// mainThread = std::thread(std::this_thread::get_id());

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0) {
		throw "Error initializing SDL subsystems : " + std::string(SDL_GetError());
	}

	WindowParams params = { "Myztic Engine Test", initScene, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
	Window* window = Window::create(params);

	SDL_GL_MakeCurrent(window->handle, window->context);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	CHECK_GL(glViewport(0, 0, 680, 480));

	Timer::debugMeasure(myzStart, "Myztic Initialization");
	//app_loop();
}

void Application::app_loop() {
	SDL_Event e;

	while (!shouldClose) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) shouldClose = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Application::start_winloop(Window* win) {
	// TODO: Mutex, wait for next event rotation thingamajig lols hahahahahaha amoignus
	// Update: maybe queues can make it all happen at the same time, by assigning events to when they would / shouldve happened in frame???
	// Like, 3 frames arent rendered yet, but you press a key that youd need to press in frame 2 while frame 1 hasnt finished, so it gets the time of your input, and assigns
	// the event to correct frame instead of forcing that frame to be waited on which also makes sure there are (close to) 0 ignored inputs!! 
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
	for (std::map<unsigned char, std::shared_ptr<Window>>::const_iterator it = windows.begin(); it != windows.end(); ++it)
	{
		std::cout << (int)it->first << " -> " << (std::string)*it->second.get() << "\n";
	}
}