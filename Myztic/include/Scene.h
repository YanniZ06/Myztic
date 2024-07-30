#pragma once

#include <display/Window.h>

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
	Scene();
	// Scene(const Scene& s) = default;
	Scene(Scene const&) = default; // do i need you

	//~Scene();

	/// The unique integer ID this scene is identified with.
	uint32_t id;
	/// A pointer to the Window this scene is currently loaded to
	Window* loadedWin;
	// todo: member fields

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
	 */
	virtual void finish();

	/**
	 * Called each game-loop frame while this scene is loaded to a Window.
	 * 
	 * \note This function is called before the scene is rendered.
	 * 
	 * \param dt Time passed since the last update call, in milliseconds.
	 */
	virtual void update(float dt);
};



