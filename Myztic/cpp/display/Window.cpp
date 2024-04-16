#include "pch.h"

#include <display/Window.h>
#include <Application.h>

// Logging
#include <windows.h>
#include <string>

Window* Window::create(WindowParams params) {
	std::shared_ptr<Window> window = std::make_shared<Window>(params);
	Application::windows[window->id] = window;

	return window.get();
}

Window::Window(WindowParams params) {
	name = params.name;
	scene = params.init_scene;

	if (!params.x) x = SDL_WINDOWPOS_CENTERED;
	else x = params.x.value();

	if (!params.y) y = SDL_WINDOWPOS_CENTERED;
	else y = params.y.value();

	if (!params.w) w = 720;
	else w = params.w.value();

	if (!params.h) h = 480;
	else h = params.h.value();

	fpsSize frames = 60;
	if (params.fps) frames = params.fps.value();
	fps = Fps(frames);

	handle = SDL_CreateWindow(name, x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!handle) {
		printf("Failed to create window\n");
	}
	context = SDL_GL_CreateContext(handle);
	id = SDL_GetWindowID(handle);
}

Window::~Window() {
	SDL_DestroyWindow(handle);
	SDL_GL_DeleteContext(context); //todo: THE RENDERER SHOULD DO THIS, NOT THE WINDOW!
	OutputDebugStringA("Deleted window\n");
	// delete handle;
	// delete name;
}

const Scene* Window::getLoadedScene() {
	return scene;
}