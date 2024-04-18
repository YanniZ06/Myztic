#pragma once

#include <SDL.h>

#include <display/Fps.h>
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

	inline std::string name();
	inline void setName(std::string v);
	inline int x();
	inline void setX(int v);
	inline int y();
	inline void setY(int v);
	inline int w();
	inline void setW(int v);
	inline int h();
	inline void setH(int v);


	// Fps settings for this window
	Fps fps;
	
	// SDL Window ID
	int id;
	// SDL Window Handle
	SDL_Window* handle;
	// SDL OpenGl Context associated with this Window
	SDL_GLContext context;

	// Returns the scene currently loaded to this Window
	Scene* getLoadedScene();

	// Sets the whole position of the window in one call.
	inline void setPosition(int x, int y);
	// Centers the windows position on an axis.
	inline void centerPosition(bool x, bool y);
	// Sets the whole size of the window in one call.
	inline void setSize(int w, int h);

	// String representation for printing out windows to the console
	explicit operator std::string();

protected:
	Scene* scene;
};