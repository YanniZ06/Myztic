#pragma once

#include <SDL.h>
class Window; //fwd declare

enum MouseButton {
	MB_NONE = 0,
	MB_LEFT = 1,
	MB_MIDDLE = 2,
	MB_RIGHT = 3,
	/// First side button on the mouse (if available)
	MB_SIDEX1 = 4,
	/// Second side button on the mouse (if available)
	MB_SIDEX2 = 5
};

struct MouseMoveEvent {
	/// The window with focus, or nullptr if there is none
	Window* focusWin;
	/// New x coordinate, relative to the focused(?) window
	Sint32 x;
	/// New y coordinate, relative to the focused(?) window
	Sint32 y;
	/// The relative motion in the X direction
	Sint32 xRel;
	/// The relative motion in the Y direction
	Sint32 yRel;
	MouseMoveEvent(Window* fw, Sint32 x, Sint32 y, Sint32 xRel, Sint32 yRel) :focusWin(fw), x(x), y(y), xRel(xRel), yRel(yRel) {}
};
