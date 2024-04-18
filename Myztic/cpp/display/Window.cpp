#include "pch.h"

#include <display/Window.h>
#include <Application.h>

// Logging
#include <windows.h>

Window* Window::create(WindowParams params) {
	std::shared_ptr<Window> window = std::make_shared<Window>(params);
	Application::windows[window->id] = window;

	return window.get();
}

Window::Window(WindowParams params) {
	_name = params.name;
	scene = params.init_scene;

	if (!params.x) _x = SDL_WINDOWPOS_CENTERED;
	else _x = params.x.value();

	if (!params.y) _y = SDL_WINDOWPOS_CENTERED;
	else _y = params.y.value();

	if (!params.w) _w = 720;
	else _w = params.w.value();

	if (!params.h) _h = 480;
	else _h = params.h.value();

	fpsSize frames = 60;
	if (params.fps) frames = params.fps.value();
	fps = Fps(frames);

	handle = SDL_CreateWindow(_name.c_str(), _x, _y, _w, _h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!handle) {
		printf("Failed to create window\n");
	}
	context = SDL_GL_CreateContext(handle);
	id = SDL_GetWindowID(handle);
	SDL_GetWindowPosition(handle, &_x, &_y);
}

Window::~Window() {
	SDL_DestroyWindow(handle);
	SDL_GL_DeleteContext(context); //todo: THE RENDERER SHOULD DO THIS, NOT THE WINDOW!
	OutputDebugStringA("Deleted window\n");
	// delete handle;
	// delete name;
}

Scene* Window::getLoadedScene() {
	return scene;
}

void Window::setPosition(int x, int y) {
	_x = x; _y = y;
	SDL_SetWindowPosition(handle, x, y);
}

void Window::centerPosition(bool x, bool y) {
	int tx = _x; int ty = _y;
	if (x) tx = SDL_WINDOWPOS_CENTERED;
	if (y) ty = SDL_WINDOWPOS_CENTERED;

	SDL_SetWindowPosition(handle, tx, ty);
	SDL_GetWindowPosition(handle, &_x, &_y);
}

void Window::setSize(int w, int h) {
	_w = w; _h = h;
	SDL_SetWindowSize(handle, w, h);
}

std::string Window::name() {
	return _name;
}

void Window::setName(std::string v) {
	_name = v;
	SDL_SetWindowTitle(handle, v.c_str());
}

int Window::x() {
	return _x;
}

void Window::setX(int v) {
	_x = v;
	SDL_SetWindowPosition(handle, v, _y);
}

int Window::y() {
	return _y;
}

void Window::setY(int v) {
	_y = v;
	SDL_SetWindowPosition(handle, _x, v);
}

int Window::w() {
	return _w;
}

void Window::setW(int v) {
	_w = v;
	SDL_SetWindowSize(handle, v, _h);
}

int Window::h() {
	return _h;
}

void Window::setH(int v) {
	_h = v;
	SDL_SetWindowSize(handle, _w, v);
}

Window::operator std::string() {
	std::string winString = "Window " +  std::to_string(id)  + "[\"" + _name + "\"] : " + std::to_string(w()) + "x " + std::to_string(h()) + " at position(" + std::to_string(x()) + " | " + std::to_string(y()) + ") running on " + std::to_string(fps.getMax()) + " max fps";
	return winString;
}