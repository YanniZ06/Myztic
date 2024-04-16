#pragma once

#include <optional>

typedef int fpsSize;

class Fps {
friend class Application;

public:
	// Creates a new Fps Handler
	Fps(fpsSize max);
	// Default Constructor, use with max argument instead
	Fps() = default;
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