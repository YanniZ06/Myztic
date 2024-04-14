#include "pch.h"

#include "display/WindowUtils.h"

// Fps

Fps::Fps(fpsSize max) {
	frameTime = 1 / max;
	_used = max;
	this->max = max;
}

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