#include "pch.h"

#include <display/Fps.h>

// Fps

Fps::Fps(fpsSize max) {
	frameTime = (1 / (float)max) * 1000;
	rawFrameTime = 1 / (float)max;
	_used = max;
	this->max = max;
}

void Fps::setMax(fpsSize max) {
	this->max = max;
	_used = max;
	frameTime = (1 / (float)max) * 1000;
	rawFrameTime = 1 / (float)max;
}