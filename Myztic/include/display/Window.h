#pragma once

#include <SDL.h>

#include <display/Fps.h>

class Scene;

struct WindowParams {
	const char* name;
	Scene* init_scene;

	std::optional<int> x, y, w, h;
	std::optional<SDL_WindowFlags> flags;
	std::optional<fpsSize> fps;
};

class Window {
private:
	const char* name;
	int x, y, w, h;

public:
	// Not meant to be called. Use Window::create instead
	Window(WindowParams params);

	static Window* create(WindowParams params);
	~Window();

	// Fps settings for this window
	Fps fps;
	
	// SDL Window ID
	int id;
	// SDL Window Handle
	SDL_Window* handle;
	// SDL OpenGl Context associated with this Window
	SDL_GLContext context;

	// Returns the scene currently loaded to this Window
	const Scene* getLoadedScene();

protected:
	Scene* scene;
};