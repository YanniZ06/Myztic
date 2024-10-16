#include "pch.h"
#include "framework.h"

#include <Application.h>
#include <graphics/Renderer.h>
#include <Scene.h>

#include <SDL.h>
#include <glad.h>

#include <Timer.h>
#include <ErrorHandler.hpp>

#define SDL_MAIN_HANDLED

// Statics
std::map<unsigned int, Window*> Application::windows;
std::binary_semaphore* resourceManager;
std::binary_semaphore* waiter;
std::atomic<uint8_t> Application::readyWinThreads = 0;
uint8_t Application::registeredWinThreads = 0;
Fps Application::fps;
bool Application::shouldClose = false;
std::binary_semaphore* Application::waiter;
ResourceManager* Application::resourceManager;

void Application::initMyztic(WindowParams initWindowParams, fpsSize fps) {
	double myzStart = Timer::stamp();
	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0) {
		throw "(MYZTIC_INIT_SDL_ERROR) Error initializing SDL subsystems : " + std::string(SDL_GetError());
	}

	const std::map<SDL_GLattr, int> GL_ATTRIBS{ {SDL_GL_RED_SIZE, 5}, {SDL_GL_GREEN_SIZE, 5}, {SDL_GL_BLUE_SIZE, 5}, {SDL_GL_DEPTH_SIZE, 16}, {SDL_GL_DOUBLEBUFFER, 1} };

	for (std::map<SDL_GLattr, int>::const_iterator it = GL_ATTRIBS.begin(); it != GL_ATTRIBS.end(); ++it) {
		SDL_GL_SetAttribute(it->first, it->second);
	}

	Window* window = new Window(initWindowParams);
	Application::windows[window->id()] = window;

	Application::fps = Fps(fps);

	SDL_GL_MakeCurrent(window->handle, window->context);

	int gladResult = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	if (gladResult == 0) {
		throw "Failed to initialize GLAD! Most likely outdated OpenGL Version, Required: OpenGL 3.3, ERROR CODE: " + std::to_string(gladResult) + "\n";
	}

	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << version << "\n";

	CHECK_GL(glViewport(0, 0, 680, 480));

	window->loadScene(initWindowParams.init_scene);
	window->switchScene(initWindowParams.init_scene);

	waiter = new std::binary_semaphore(0);
	resourceManager = new ResourceManager();

	Timer::debugMeasure(myzStart, "Myztic Initialization");
	app_loop();
}

void Application::app_loop() {
	SDL_Event e;

	while (!shouldClose) {
		//! std::cout << "(63) AppLoop Instance \n";
		// Step 1: Check for and handle all sorts of SDL events, such as inputs or window actions
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			// Handle Window Events
			case SDL_WINDOWEVENT: {
				Window* eWin = Application::windows[e.window.windowID];

				if (eWin == nullptr) {
					//should be null anyway, we dont WANT THAT HERE
					Application::windows.erase(e.window.windowID);
					break;
				}

				//todo: dispatch window events once an event system has been made
				switch (e.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					eWin->shouldClose = true;
					registeredWinThreads--;
					eWin->thread.signal->release(); // Should kill the thread because the loop condition is no longer met
					
					std::cout << "Wanting to close window: " << eWin->id() << "\n";

					eWin->destroy();
					delete eWin;

					log_windows_cmd();
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					eWin->_focused = true;
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					eWin->_focused = false;
					break;
				case SDL_WINDOWEVENT_RESIZED:
					//TODO: handle resizing on renderer hahahah
					//? ZIAD JESUS CHRIST I SAID ON RENDERER OH MY GODDDDD
					SDL_GL_MakeCurrent(eWin->handle, eWin->context);
					CHECK_GL(glViewport(0, 0, e.window.data1, e.window.data2));
					break;
				default: break;
				}

				break;
			}
			// Other Event Types
			case SDL_QUIT:
				shouldClose = true;
				break;
			default:
				break;
			}
		}

		// Step 2: Start & continue all winloops -> create drawing requests for the next frame and handle physics
		for (std::map<unsigned int, Window*>::const_iterator it = windows.begin(); it != windows.end(); ++it) {
			Window* win = it->second;
			win->thread.signal->release();

			// Start rendering <3 (Handle draw requests (maybe manage to handle the last frames draw requests while this frame recommends a new one so this thread isnt wasted just sleeping?))
			win->renderer.startRender();

			//presents shit to screen
			win->renderer.endRender();
		}

		// Unless we are somehow already ready to continue (edge-case), pause current thread, gets unpaused once the last thread is ready
		if (readyWinThreads.load() < registeredWinThreads) { 
			waiter->acquire(); 
		}
		
		readyWinThreads.store(0, std::memory_order_relaxed);
		// Finally, wait the rest of the frame or continue right away (do math here or some shit
		SDL_Delay(1);
	}
}

// Update: maybe queues can make it all happen at the same time, by assigning events to when they would / shouldve happened in frame???
// Like, 3 frames arent rendered yet, but you press a key that youd need to press in frame 2 while frame 1 hasnt finished, so it gets the time of your input, and assigns
// the event to correct frame instead of forcing that frame to be waited on which also makes sure there are (close to) 0 ignored inputs!! 

void Application::start_winloop(Window* win) {
	if (win->shouldClose) return;
	win->thread.signal->acquire();

	registeredWinThreads++;
	window_loop(win);
}

// Todo: Run soely physics and drawing REQUESTS to the drawing (currently main) thread in here | Unlikely -> preloading is done on ANOTHER thread if we do the context preloading thing
void Application::window_loop(Window* win) {
	while (!win->shouldClose) {		
		win->scene->update((float)fps.getFrameTime()); // Put elapsed time in here, for now it gives you the max framerate elapsed

		readyWinThreads++;
		if (readyWinThreads.load() == registeredWinThreads) { // Continue main thread if this was the last window thread that finished
			waiter->release();
		}
		win->thread.signal->acquire(); // Block execution until this window thread is released by main thread
	}
}

// Logging and shit (remove for end releases????)

void Application::log_windows_cmd() {
	std::cout << "Application::windows =>\n";
	for (std::map<unsigned int, Window*>::const_iterator it = windows.begin(); it != windows.end(); ++it)
	{
		std::cout << (int)it->first << " -> " << (std::string)*it->second << "\n";
	}
}