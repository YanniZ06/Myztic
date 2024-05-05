#include "pch.h" 

#include <Scene.h>
#include <Util.h>

Scene::Scene() {
	id = IDGenerator::instance()->next();
}
// Scene::~Scene() {}

void Scene::load(Window* callerWindow) {}
void Scene::unload(Window* callerWindow) {}
void Scene::update(float dt) {}