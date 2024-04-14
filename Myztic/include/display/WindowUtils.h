#pragma once

#include "Application.h"
#include "Scene.h"

#include <optional>

typedef int fpsSize;

class Fps {
friend class Application;

public:
	// Creates a new Fps Handler
	Fps(fpsSize max);
	// Destroys the Fps Handler
	//~Fps();


	//Gets the max fps cap
	fpsSize getMax();
	// Sets the max fps cap
	void setMax(fpsSize max);
	// MS between each frame at max framerate
	float getFrameTime();
	// The fps actually used
	fpsSize used();
	
protected:
	fpsSize _used;
	fpsSize max;

	float frameTime;
};

// todo: return to typedef struct
struct WindowParams {
	const char* name;
	Scene* init_scene;

	std::optional<int> x, y, w, h;
	std::optional<SDL_WindowFlags> flags;
	std::optional<fpsSize> fps;
};
