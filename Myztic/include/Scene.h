#pragma once

#include <stdint.h>
#include <vector>

namespace Myztic {
	class Window;
	class Camera;

	/**
	 * The base for any scene.
	 *
	 * Scenes are used to allow for a controlled and easy workflow.
	 * \n They are containers for renderable objects and can be loaded onto any Window created by Myztic.
	 * \n Every scene has 3 basic components:
	 *	1. **The loading functions** \n Used to preload scene assets without calling any other scene logic
	 *  2. **The displaying functions** \n Used to signal when the scene should start displaying its contents and start its update loop
	 *  3. **The dedicated update loop** \n Used to handle physics and other related logic that needs to be updated constantly
	 *
	 * Each of these components are meant to be overriden per scene.
	 * \n Every custom scene must extend this base scene class to function.
	 */
	class Scene {
	public:
		/**
		 * Constructs your scene and sets your tag to "unnamed".
		 */
		Scene();
		/**
		 * Constructs your scene whilst allowing you to set its tag.
		 *
		 * \param name Sets the tag for this scene, to allow resolving it by name. If multiple instances of the same scene exist and share the same tag you must use getId() instead.
		 */
		Scene(const char* name);
		~Scene();

		/// String name-tag for the scene
		const char* tag;
		/// A pointer to the Window this scene is currently loaded to
		Window* loadedWin;
		// todo: member fields

		/// Returns the unique integer ID this scene is identified with.
		inline uint32_t getId() {
			return id;
		};
		/**
		 * Called when this scene is being loaded into a windows scene cache.
		 *
		 * \param callerWindow Window this scene is being loaded to.
		 */
		virtual void load(Window* callerWindow);

		/**
		 * Called when this scene is being removed from a windows scene cache.
		 *
		 * \param callerWindow Window this scene is being unloaded from.
		 */
		virtual void unload(Window* callerWindow);

		/**
		 * Called when this scene is switched to a Windows' active scene.
		 */
		virtual void enter();
		/**
		 * Called when this scene is overriden and becomes inactive.
		 *
		 * \param nextScene The scene that will be switched to.
		 */
		virtual void finish(Scene* nextScene);

		/**
		 * Called each game-loop frame while this scene is loaded to a Window.
		 *
		 * \note This function is called before the scene is rendered.
		 *
		 * \param dt Time passed since the last update call, in milliseconds.
		 */
		virtual void update(float dt);

		/*
		* An array of cameras, as each scene object might link to a camera, if it does, it's rendered in either a perspective or an orthographic manner.
		* If an object isn't linked to a camera, it's rendered directly onto the screen (most likely 2D HUD objects).
		*/
		std::vector<Camera*> cameras = std::vector<Camera*>();

		/**
		 * Main camera that almost the entirety of the scene is rendered in.
		*/
		Camera* mainCamera = nullptr;
	protected:
		uint32_t id;
	};
}