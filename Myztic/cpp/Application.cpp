#include "pch.h"
#include "framework.h"

#include <Application.h>
#include <Audio.h>
#include <events/EventDispatcher.h>
#include <display/Window.h>
#include <graphics/Renderer.h>
#include <Scene.h>

#include <SDL.h>
#include <glad.h>
#include <alext.h>


#include <Timer.h>
#include <ErrorHandler.hpp>
#include <glm\gtc\quaternion.hpp>
#include <graphics\PrecompiledShaders.h>
#include <graphics\Camera.h>

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
bool freeCamera = false;

void Application::initMyztic(WindowParams& initWindowParams, fpsSize fps) {
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

	PrecompiledShaders::Compile();

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

				//todo: dispatch window events once an event system has been made (HAHAHAHAHAHAHAHAHAHAHHAHAH guess what)
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
					SDL_GL_MakeCurrent(eWin->handle, eWin->context);
					CHECK_GL(glViewport(0, 0, e.window.data1, e.window.data2));
					break;
				default: break;
				}

				break;
			}
			// Mouse Events
			/*case SDL_MOUSEMOTION:
				Window* eWin = nullptr;
				if (windows.count(e.motion.windowID)) eWin = windows[e.motion.windowID];

				break;
			*/
			// Other Event Types
			case SDL_QUIT:
				shouldClose = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
					case SDLK_INSERT:
						freeCamera = !freeCamera;
						SDL_CaptureMouse(freeCamera ? SDL_TRUE : SDL_FALSE);
						SDL_SetRelativeMouseMode(freeCamera ? SDL_TRUE : SDL_FALSE);
						break;
					//this is fucking horrible i should find another way to do this
					case SDLK_w:
						if (windows.count(e.key.windowID)) {
							Window* win = windows[e.key.windowID];
							if (win->scene != nullptr && freeCamera) {
								for (Camera* cam : win->scene->cameras) {
									cam->set_position(cam->cam_position + (cam->look_at * (fps.rawFrameTime * 7.f)));
								}
							}
						}
						break;
					case SDLK_s:
						if (windows.count(e.key.windowID)) {
							Window* win = windows[e.key.windowID];
							if (win->scene != nullptr && freeCamera) {
								for (Camera* cam : win->scene->cameras) {
									cam->set_position(cam->cam_position - (cam->look_at * (fps.rawFrameTime * 7.f)));
								}
							}
						}
						break;
					case SDLK_a:
						if (windows.count(e.key.windowID)) {
							Window* win = windows[e.key.windowID];
							if (win->scene != nullptr && freeCamera) {
								for (Camera* cam : win->scene->cameras) {
									 cam->set_position(cam->cam_position - (glm::normalize(glm::cross(cam->look_at, Camera::UP)) * (fps.rawFrameTime * 7.f)));
								}
							}
						}
						break;
					case SDLK_d:
						if (windows.count(e.key.windowID)) {
							Window* win = windows[e.key.windowID];
							if (win->scene != nullptr && freeCamera) {
								for (Camera* cam : win->scene->cameras) {
									cam->set_position(cam->cam_position + (glm::normalize(glm::cross(cam->look_at, Camera::UP)) * (fps.rawFrameTime * 7.f)));
								}
							}
						}
						break;
					default: break;
				}
				break;
			// /*
			case SDL_MOUSEMOTION:
				if (windows.count(e.motion.windowID)) {
					Window* win = windows[e.motion.windowID];
					if (win->scene != nullptr && freeCamera) {
						for (Camera* cam : win->scene->cameras) {
							//to look left and right, we must modify the yaw axis by a certain variable according to the delta x (xrel) value
							glm::vec3 lookDirection = glm::vec3();
							glm::vec3 look_at = cam->get_look_at();
							cam->yawAngle += e.motion.xrel;
							cam->pitchAngle = glm::clamp(cam->pitchAngle - e.motion.yrel, -89.0f, 89.0f);
							lookDirection.x = cos(glm::radians(cam->yawAngle)) * cos(glm::radians(cam->pitchAngle));
							lookDirection.y = sin(glm::radians(cam->pitchAngle));
							lookDirection.z = sin(glm::radians(cam->yawAngle)) * cos(glm::radians(cam->pitchAngle));
							cam->set_look(glm::normalize(lookDirection));
						}
					}
				}
				
				break;
			//	*/
			/*
			case SDL_USEREVENT:
				if (e.user.type != Audio::systemEvents.OPENAL_SYSTEM_EVENT_SDLEVENTTYPE) break; // Queued OpenAL system event

				// To obtain event-related device names we increment the system message up until where the device name is mentioned
				switch ((int)e.user.data1) { // todo: replace first arg of callback with affected device name (using offs
				case ALC_EVENT_TYPE_DEVICE_ADDED_SOFT:
					Audio::systemEvents.onDeviceAdded.callback((const char*)e.user.data2, (DeviceType)e.user.code);
					// refresh playback list

					break;
				case ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT:
					// refresh playback list, potentially retarget device (if removed device was the current one)

					break;
				case ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT: {
					if ((DeviceType)e.user.code == PLAYBACK_DEVICE) {
						const char* oldDeviceName = Audio::defaultPbd;
						const char* newDeviceName = (const char*)e.user.data2 + 33;

						//todo: maybe directly set defaultPbd using newDeviceName (if that is a proper device name)
						//Audio::refreshDefaultPlaybackDevice();

						Audio::systemEvents.onDefaultDeviceChanged.callback(oldDeviceName, newDeviceName, PLAYBACK_DEVICE);


						break;
					}

					const char* oldDeviceName = Audio::defaultMic;
					const char* newDeviceName = (const char*)e.user.data2 + 0; // todo: get offset

					Audio::systemEvents.onDefaultDeviceChanged.callback(oldDeviceName, newDeviceName, PLAYBACK_DEVICE);
					// transmit event prior to refresh, pass in Audio::getDefaultPlaybackDevice as first parameter, probably leave device retarget to programmer

					break;
				}
				
				default:
					break;
				}
			*/
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