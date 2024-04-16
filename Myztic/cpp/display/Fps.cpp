#include "pch.h"

#include <display/Fps.h>

// Fps

Fps::Fps(fpsSize max) {
	frameTime = 1 / float(max);
	_used = max;
	this->max = max;
}

fpsSize Fps::getMax() { 
	return max; 
}

void Fps::setMax(fpsSize max) {
	this->max = max;
	_used = max;
	frameTime = 1 / float(max);
}

fpsSize Fps::used() {
	return _used;
}

float Fps::getFrameTime() {
	return frameTime;
}