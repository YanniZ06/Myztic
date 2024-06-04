#pragma once

#include <display/Window.h>

// The base for any scene. Meant to be inherited from.
class Scene {
public:
	Scene();
	// Scene(const Scene& s) = default;
	Scene(Scene const&) = default; // do i need you

	//~Scene();

	unsigned int id;
	Window* loadedWin;
	// todo: member fields

	// Called when this scene is loaded to a Window
	virtual void load(Window* callerWindow);
	// Called when this scene is unloaded from a Window
	virtual void unload(Window* callerWindow);
	// Called when this scene is switched to a Windows' active scene
	virtual void enter();
	// Called when this scene is overriden and becomes inactive
	virtual void finish();

	// Called per-frame while this scene is loaded to a Window
	virtual void update(float dt);
};