#include "pch.h"

#include <display/Fps.h>

using namespace Myztic;

Fps::Fps(fpsSize max) {
	frameTime = (1 / (float)max) * 1000;
	rawFrameTime = 1 / (float)max;
	_used = static_cast<double>(max);
	this->max = max;
}

void Fps::setMax(fpsSize max) {
	this->max = max;
	_used = static_cast<double>(max);
	frameTime = (1 / (float)max) * 1000;
	rawFrameTime = 1 / (float)max;
}