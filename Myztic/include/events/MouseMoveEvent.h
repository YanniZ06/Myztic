#pragma once

struct MouseMoveEvent {
	int x;
	int y;
	MouseMoveEvent(int x, int y) : x(x), y(y) {}
};
