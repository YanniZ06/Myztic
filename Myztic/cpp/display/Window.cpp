#include "pch.h"

#include <Application.h>
#include <Scene.h>
#include <display/Window.h>


// Logging
#include <windows.h>
#include <iostream>

using namespace Myztic;

Window* Window::create(WindowParams params) {
	Window* window = new Window(params);
	Application::windows[window->id()] = window;

	window->loadScene(params.init_scene);
	window->switchScene(params.init_scene);

	return window;
}

Window::Window(WindowParams params) {
	_name = params.name;
	scene = nullptr;
	shouldClose = false;

	if (!params.x) _x = SDL_WINDOWPOS_CENTERED;
	else _x = params.x.value();

	if (!params.y) _y = SDL_WINDOWPOS_CENTERED;
	else _y = params.y.value();

	if (!params.w) _w = 720;
	else _w = params.w.value();

	if (!params.h) _h = 480;
	else _h = params.h.value();

	handle = SDL_CreateWindow(_name.c_str(), _x, _y, _w, _h, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	if (!handle) {
		printf("Failed to create window\n");
	}
	context = SDL_GL_CreateContext(handle);
	_id = SDL_GetWindowID(handle);
	SDL_GetWindowPosition(handle, &_x, &_y);

	_focused = (SDL_GetWindowFlags(handle) & SDL_WINDOW_INPUT_FOCUS) != 0;
	renderOutOfFocus = true;

	renderer = Renderer(this);
	thread = WinThread(this);
	// No code beyond here, the thread has been moved.
}

Window::~Window() {
	std::cout << "deconstructing window " << std::to_string(SDL_GetWindowID(handle)) << "\n";

	scene = nullptr;
	context = nullptr;
	handle = nullptr;
	// renderer.~Renderer();
}

bool Window::switchScene(Scene* inputScene)
{
	if (this->scene) this->scene->finish(inputScene);
	if (inputScene->loadedWin != this) return false;

	inputScene->enter();
	this->scene = inputScene;
	return true;
}

bool Window::loadScene(Scene* inputScene) {
	if (inputScene->loadedWin) return false;

	inputScene->loadedWin = this;
	inputScene->load(this);
	//std::shared_ptr<Scene> ss = std::shared_ptr<Scene>(scene);
	loadedScenes[inputScene->getId()] = inputScene;
	return true;
}

bool Window::unloadScene(Scene* inputScene, bool freeMem) {
	if (inputScene->loadedWin != this) return false;
	if (inputScene == this->scene) { // Make sure we break nothing if there is no current scene 
		Scene* placeHolderScene = new Scene();
		this->scene->finish(placeHolderScene);
		this->scene = placeHolderScene;
	}

	inputScene->unload(this);
	inputScene->loadedWin = nullptr;
	loadedScenes.erase(inputScene->getId());

	if (freeMem) delete inputScene;
	return true;
}

void Window::centerPosition(bool x, bool y) {
	int tx = _x; int ty = _y;
	if (x) tx = SDL_WINDOWPOS_CENTERED;
	if (y) ty = SDL_WINDOWPOS_CENTERED;

	SDL_SetWindowPosition(handle, tx, ty);
	SDL_GetWindowPosition(handle, &_x, &_y);
}

Window::operator std::string() {
	//doing Application::fps here for now, every window is gonna have it's own independent FPS instance later // WE ARE NOT DOING THAT ANYMORE
	std::string winString = "Window " +  std::to_string(id())  + "[\"" + _name + "\"] : " + std::to_string(w()) + "x " + std::to_string(h()) + " at position(" + std::to_string(x()) + " | " + std::to_string(y()) + ") running on " + std::to_string(Application::fps.getMax()) + " max fps";
	return winString;
}

// Private Cleanup
void Window::destroy()
{
	// Unload all scenes, including current one
	Application::windows.erase(id());
	std::map<unsigned int, Scene*>::const_iterator it = loadedScenes.begin();
	for (int i = 0; i < loadedScenes.size(); i++) {
		this->unloadScene(it++->second);
	}
	std::cout << "destroying window " << std::to_string(SDL_GetWindowID(handle)) << "\n";
	delete scene;

	SDL_GL_DeleteContext(context); //todo: THE RENDERER SHOULD DO THIS, NOT THE WINDOW?
	SDL_DestroyWindow(handle);

	renderOutOfFocus = false;
	inRenderPhase = false;

	thread.destroy();
	// this->~Window(); is called by the delete operator in Application.cpp
}
