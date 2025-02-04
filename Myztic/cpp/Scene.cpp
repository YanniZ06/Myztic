#include "pch.h" 
#include <Scene.h>
#include <display\Window.h>
#include <graphics\Camera.h>
#include <utilities/IDGenerator.h>

Myztic::Scene::Scene() {
	id = IDGenerator::instanceOf("scene")->next();
	tag = "unnamedScene";
	loadedWin = nullptr;
}

Myztic::Scene::Scene(const char* name)
{
	id = IDGenerator::instanceOf("scene")->next();
	tag = name;
	loadedWin = nullptr;
}

Myztic::Scene::~Scene() {
	loadedWin = nullptr;
}

void Myztic::Scene::load(Window* callerWindow) {}
void Myztic::Scene::unload(Window* callerWindow) {}
void Myztic::Scene::enter() {}
void Myztic::Scene::finish(Scene* nextScene) {}
void Myztic::Scene::update(float dt) {}
