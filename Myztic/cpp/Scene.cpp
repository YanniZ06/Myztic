#include "pch.h" 
#include <Scene.h>
#include <display\Window.h>
#include <graphics\Camera.h>
#include <utilities/IDGenerator.h>

Scene::Scene() {
	id = IDGenerator::instanceOf("scene")->next();
	tag = "unnamedScene";
	loadedWin = nullptr;
}

Scene::Scene(const char* name)
{
	id = IDGenerator::instanceOf("scene")->next();
	tag = name;
	loadedWin = nullptr;
}

Scene::~Scene() {
	loadedWin = nullptr;
}

void Scene::load(Window* callerWindow) {}
void Scene::unload(Window* callerWindow) {}
void Scene::enter() {}
void Scene::finish(Scene* nextScene) {}
void Scene::update(float dt) {}
