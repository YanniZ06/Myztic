#include "pch.h"
#include "include/display/WindowUtils.h"

// Fps

Fps::Fps(fpsSize maxDesired) {}

fpsSize Fps::getMax() { 
	return max; 
}

void Fps::setMax(fpsSize max) {
	this->max = max;
	_used = max;
	frameTime = 1 / max;
}

fpsSize Fps::used() {
	return _used;
}

float Fps::getFrameTime() {
	return frameTime;
}

