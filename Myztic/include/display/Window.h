#pragma once
#include "SDL.h"

#include "display/WindowUtils.h"

class Window {
private:
	const char* name;
	int x, y, w, h;

public:
	static Window* create(struct WindowParams params);
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
	Window(struct WindowParams params);
	Scene* scene;
};