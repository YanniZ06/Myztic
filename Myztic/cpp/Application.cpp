#include "pch.h"
#include "framework.h"

#include <Application.h>
#include <Scene.h>

#include <SDL.h>
#include <glad.h>

#include <Timer.h>
#include <ErrorHandler.hpp>

#define SDL_MAIN_HANDLED

// Statics
std::map<unsigned char, std::shared_ptr<Window>> Application::windows;
std::thread Application::mainThread;
std::binary_semaphore* resourceManager;
std::binary_semaphore* waiter;
unsigned char Application::readyWinThreads = 0;
unsigned char Application::registeredWinThreads = 0;
Fps Application::fps;
bool Application::shouldClose = false;
std::binary_semaphore* Application::waiter;
ResourceManager Application::resourceManager;

void Application::initMyztic(WindowParams initWindowParams, fpsSize fps) {
	mainThread = std::thread(_initMyztic, initWindowParams, fps);
}

void Application::_initMyztic(WindowParams p, fpsSize fps) {
	double myzStart = Timer::stamp();
	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0) {
		throw "Error initializing SDL subsystems : " + std::string(SDL_GetError());
	}

	Window* window = Window::create(p);
	Application::fps = Fps(fps);

	SDL_GL_MakeCurrent(window->handle, window->context);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	CHECK_GL(glViewport(0, 0, 680, 480));

	waiter = new std::binary_semaphore(0);
	resourceManager = ResourceManager();

	Timer::debugMeasure(myzStart, "Myztic Initialization");
	app_loop();
}
int decrementTimes = 0;

void Application::app_loop() {
	SDL_Event e;
	//? this actually isnt right, i cant move window or do any window operation on neither (NOTE: That was because waiter functionality-
	//? hasn't actually been implemented yet, although when I comment out lines 73, 74, i can move windows, i can't exit which just means-
	//? that Step 2 isn't happening correctly or for some reason, event's aren't being handled?)
	
	while (!shouldClose) {
		// Step 1: Check for and handle all sorts of SDL events, such as inputs or window actions
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				shouldClose = true; 
			}
		}

		// Step ?: Handle draw requests
		 
		 
		// Step 2: Start & continue all winloops -> create drawing requests for the next frame and handle physics
		for (std::map<unsigned char, std::shared_ptr<Window>>::const_iterator it = windows.begin(); it != windows.end(); ++it) {
			it->second.get()->thread.signal->release();
		}

		if (readyWinThreads < registeredWinThreads) { 
			decrementTimes++;
			std::cout << "waiter acquired, decremented: " + std::to_string(decrementTimes) << "\n";
			
			waiter->acquire(); 
		}
		
		readyWinThreads = 0;
		// Finally, wait the rest of the frame or continue right away
		SDL_Delay(1);
	}
}

// Update: maybe queues can make it all happen at the same time, by assigning events to when they would / shouldve happened in frame???
// Like, 3 frames arent rendered yet, but you press a key that youd need to press in frame 2 while frame 1 hasnt finished, so it gets the time of your input, and assigns
// the event to correct frame instead of forcing that frame to be waited on which also makes sure there are (close to) 0 ignored inputs!! 

void Application::start_winloop(Window* win) {
	win->thread.signal->acquire();

	registeredWinThreads++;
	window_loop(win);
}
int incrementTimes = 0;

// Todo: Run soely physics and drawing REQUESTS to the drawing (currently main) thread in here | Unlikely -> preloading is done on ANOTHER thread if we do the context preloading thing
void Application::window_loop(Window* win) {
	while (!win->shouldClose) {
		SDL_Delay(1);
		win->scene->update(fps.getFrameTime()); // Put elapsed time in here, for now it gives you the max framerate elapsed

		readyWinThreads++;
		//? waiter is never released
		if (readyWinThreads == registeredWinThreads) {
			incrementTimes++;
			std::cout << "waiter released, incremented: " + std::to_string(incrementTimes) << "\n";

			waiter->release(); 
		}
		win->thread.signal->acquire(); // Block execution until thread is released by main thread
	}
}

// Logging and shit

void Application::log_windows_cmd() {
	std::cout << "Application::windows =>\n";
	for (std::map<unsigned char, std::shared_ptr<Window>>::const_iterator it = windows.begin(); it != windows.end(); ++it)
	{
		std::cout << (int)it->first << " -> " << (std::string)*it->second.get() << "\n";
	}
}