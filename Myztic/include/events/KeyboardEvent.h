#pragma once

#include <SDL.h>

namespace Myztic {
	class Window; //fwd declare

	/**
	 * Fired when any activity related to the keyboard has been logged (key press/key release)
	 */
	struct KeyboardEvent {
		/// The window with focus, or nullptr if there is none
		Window* focusWin;

		/// The type of keyboard even. If true a key press, otherwise a key release.
		bool keyDown;

		/// The physical key representation independent of language settings.
		SDL_Scancode physicalKey;

		/// The key that was pressed or released
		SDL_Keycode key;

		KeyboardEvent(Window* fw, bool keydown, SDL_Scancode pk, SDL_Keycode k) :focusWin(fw), keyDown(keydown), physicalKey(pk), key(k) {}
	};
}