#pragma once

#include <SDL.h>

#include <display/Fps.h>
#include <thread>
#include <string>

class Scene;

struct WindowParams {
	std::string name;
	Scene* init_scene;

	std::optional<int> x, y, w, h;
	std::optional<SDL_WindowFlags> flags;
	std::optional<fpsSize> fps;
};

class Window {
private:
	std::string _name;
	int _x, _y, _w, _h;

public:
	// Not meant to be called. Use Window::create instead
	Window(WindowParams params);

	static Window* create(WindowParams params);
	~Window();

	inline std::string name() {
		return _name;
	}

	inline void setName(std::string v) {
		_name = v;
		SDL_SetWindowTitle(handle, v.c_str());
	}

	// Current x coordinate of the window in pixels
	inline int x() {
		return _x;
	}

	// Sets the x coordinate of the window in pixels
	inline void setX(int v) {
		_x = v;
		SDL_SetWindowPosition(handle, v, _y);
	}

	// Current y coordinate of the window in pixels
	inline int y() {
		return _y;
	}

	// Sets the y coordinate of the window in pixels
	inline void setY(int v) {
		_y = v;
		SDL_SetWindowPosition(handle, _x, v);
	}

	// Current width of the window in pixels
	inline int w() {
		return _w;
	}

	// Sets the width of the window in pixels
	inline void setW(int v) {
		_w = v;
		SDL_SetWindowSize(handle, v, _h);
	}

	// Current height of the window in pixels
	inline int h() {
		return _h;
	}

	// Sets the height of the window in pixels
	inline void setH(int v) {
		_h = v;
		SDL_SetWindowSize(handle, _w, v);
	}

	bool shouldClose;

	// Fps settings for this window
	Fps fps;
	
	// SDL Window ID
	int id;
	// SDL Window Handle
	SDL_Window* handle;
	// SDL OpenGl Context associated with this Window
	SDL_GLContext context;
	// The thread for this window
	std::thread thread;

	// Returns the scene currently loaded to this Window
	Scene* getLoadedScene();

	// Sets the whole position of the window in one call.
	inline void setPosition(int x, int y) {
		_x = x; _y = y;
		SDL_SetWindowPosition(handle, x, y);
	}

	// Centers the windows position on an axis.
	void centerPosition(bool x, bool y);
	// Sets the whole size of the window in one call.
	inline void setSize(int w, int h) {
		_w = w; _h = h;
		SDL_SetWindowSize(handle, w, h);
	}

	// String representation for printing out windows to the console
	explicit operator std::string();

protected:
	Scene* scene;
};