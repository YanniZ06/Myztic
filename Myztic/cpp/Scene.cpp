#include "pch.h" 

#include <Scene.h>
#include <Util.h>

Scene::Scene() {
	id = IDGenerator::instanceOf("scene")->next();
	loadedWin = nullptr;
}

Scene::~Scene() {
	loadedWin = nullptr;
}

void Scene::load(Window* callerWindow) {}
void Scene::unload(Window* callerWindow) {}
void Scene::enter() {}
void Scene::finish() {}
void Scene::update(float dt) {}
