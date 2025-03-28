#pragma once

#include <SDL.h>
#include <stdint.h>

#include <map>
#include <memory>
#include <optional>
#include <thread>
#include <string>

#include <display/WinThread.h>
#include <graphics/Renderer.h>

namespace Myztic {
	class Scene;

	struct WindowParams {
		/// The name for your window
		std::string name;
		/// The scene this window starts on
		Scene* init_scene;

		std::optional<int> x, y, w, h;
		/// Custom SDL_WindowFlags OR'd together, for example like `SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS | SDL_WINDOW_MINIMIZED`
		/// 
		/// You may not use the following SDL_WindowFlags: SDL_WINDOW_UTILITY, SDL_WINDOW_TOOLTIP, SDL_WINDOW_POPUP_MENU, SDL_WINDOW_VULKAN, SDL_WINDOW_METAL & SDL_WINDOW_OPENGL
		std::optional<SDL_WindowFlags> flags;
	};

	class Window {
		friend class Application;
	private:
		std::string _name;
		int _x, _y, _w, _h;
		int _id;
		bool _focused;

		std::map<unsigned int, Scene*> loadedScenes;

	public:
		/// Not meant to be called. Use Window::create instead
		Window(WindowParams params);

		static Window* create(WindowParams params);
		~Window();
		/**
		 * Closes the window on the next frame and cleans it up.
		 *
		 * This window cannot be used or referenced after this function has been called.
		 *
		 */
		inline void close() {
			SDL_Event e;
			e.type = SDL_WINDOWEVENT;
			e.window.type = SDL_WINDOWEVENT_CLOSE;
			e.window.windowID = id();

			SDL_PushEvent(&e);
		}

		inline std::string name() {
			return _name;
		}

		inline void setName(std::string v) {
			_name = v;
			SDL_SetWindowTitle(handle, v.c_str());
		}

		// Current x coordinate of the window in pixels
		inline int x() {
			return _x;
		}

		// Sets the x coordinate of the window in pixels
		inline void setX(int v) {
			_x = v;
			SDL_SetWindowPosition(handle, v, _y);
		}

		// Current y coordinate of the window in pixels
		inline int y() {
			return _y;
		}

		// Sets the y coordinate of the window in pixels
		inline void setY(int v) {
			_y = v;
			SDL_SetWindowPosition(handle, _x, v);
		}

		// Current width of the window in pixels
		inline int w() {
			return _w;
		}

		// Sets the width of the window in pixels
		inline void setW(int v) {
			_w = v;
			SDL_SetWindowSize(handle, v, _h);
		}

		// Current height of the window in pixels
		inline int h() {
			return _h;
		}

		// Sets the height of the window in pixels
		inline void setH(int v) {
			_h = v;
			SDL_SetWindowSize(handle, _w, v);
		}

		inline bool focused() { return _focused; }

		/// Dictates whether frames for this window should be rendered if the window is not in focus. True by default.
		bool renderOutOfFocus;

		// Acts as a signal for the window and thread to close
		bool shouldClose; //? Make getter/protected (atomic?)
		// Acts as a signal for the window to update and render however many frames it needs to
		bool inRenderPhase; //? Deprecated

		// SDL Window ID
		inline int id() { return _id; }

		// todo: make getters!!!!!!!!!! we need some more structure over here
		// SDL Window Handle
		SDL_Window* handle;
		// SDL OpenGl Context associated with this Window
		SDL_GLContext context;
		// The thread for this window
		WinThread thread;

		/// The renderer for this Window
		Renderer renderer;

		// Returns the scene currently displayed on this Window
		// Scene* getActiveScene();

		// Todo: test this at some point jesus christ
		// Returns a map with all the scenes currently loaded to this window
		// outSize: Amount of loaded scenes
		inline Scene** getLoadedScenes(size_t& outSize) {
			outSize = loadedScenes.size();
			Scene** scenes = new Scene * [outSize];

			std::map<unsigned int, Scene*>::const_iterator it = loadedScenes.begin();
			for (int i = 0; i < outSize; i++) {
				scenes[i] = it++->second;
			}

			return scenes;
		}

		/// Loads this scenes contents to this Window, returns false if the scene is already loaded to a Window
		bool loadScene(Scene* scene);

		/**
		 * Unloads a scenes contents from this Window. This operation calls scene->unload(this);
		 * \nIf the scene is currently the Windows' active scene, scene->finish(this); will be called & a dummy scene will be loaded.
		 *
		 * \param scene The scene whichs contents should be unloaded from the Window.
		 * \param freeMem If true, deletes the scene from memory entirely. The scene should no longer be referenced afterwards.
		 * \return True if the scene was loaded to this window, otherwise false.
		 */
		bool unloadScene(Scene* scene, bool freeMem = false);

		/// Switches this windows' active scene to the input scene, returns false if the input scene is not loaded to this window
		bool switchScene(Scene* scene);

		/// Sets the current OpenGL context to the one of this window. Should be done before directly messing with anything OpenGL related.
		inline void switchToContext() {
			SDL_GL_MakeCurrent(handle, context);
		}

		// Sets the whole position of the window in one call.
		inline void setPosition(int x, int y) {
			_x = x; _y = y;
			SDL_SetWindowPosition(handle, x, y);
		}

		// Centers the windows position on an axis.
		void centerPosition(bool x, bool y);
		// Sets the whole size of the window in one call.
		inline void setSize(int w, int h) {
			_w = w; _h = h;
			SDL_SetWindowSize(handle, w, h);
		}

		// String representation for printing out windows to the console
		explicit operator std::string();

	protected:
		void destroy();
		Scene* scene;
	};
}