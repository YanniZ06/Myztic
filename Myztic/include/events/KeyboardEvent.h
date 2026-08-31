#pragma once

#include <SDL.h>

namespace Myztic {
	class Window; //fwd declare

	/**
	 * Fired when any activity related to the keyboard has been logged (key press/key release)
	 */
	struct KeyboardEvent {
		/// The window with focus, or nullptr if there is none (this can be the case with IMGUI windows)
		Window* focusWin;

		/// The type of keyboard even. If true a key press, otherwise a key release.
		bool keyDown;

		/// The physical key representation independent of language settings.
		SDL_Scancode physicalKey;

		/// The key that was pressed or released
		SDL_Keycode key;

		/// Raw SDL_Event for misce
		SDL_Event raw_event;

		KeyboardEvent(Window* fw, bool keydown, SDL_Scancode pk, SDL_Keycode k, SDL_Event re) :focusWin(fw), keyDown(keydown), physicalKey(pk), key(k), raw_event(re) {}
	};
}