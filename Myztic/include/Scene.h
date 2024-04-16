#pragma once

#include <display/Window.h>

// The base for any scene. Meant to be inherited from.
class Scene {
public:
	Scene();
	~Scene();

	// todo: member fields

	// Called when this scene is loaded to a Window
	virtual void load(Window* callerWindow);
	// Called when this scene is unloaded from a Window
	virtual void unload(Window* callerWindow);
	// Called per-frame when this scene is loaded to a Window
	virtual void update(float dt);
};