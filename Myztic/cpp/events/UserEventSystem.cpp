#include "pch.h"

#include <events/UserEventSystem.h>
// #include <events/EventSystem.h>

using namespace Myztic;

/*
template<typename EventInfoStruct>
void EventSystem::registerEvent(EventType type, std::function<void(EventInfoStruct)> callbackFunc, uint32_t id) {
	EventHandle& evh = eventsList[type];
	evh.registered = true;

	// if (evh.callbacks.count(id) && !dupe) evh.callbacks.erase(id); // Dupe ID; unregister the event
	evh.callbacks[id].push_back([callbackFunc](void* data) {
		callbackFunc(*static_cast<EventInfoStruct*>(data));
		});
}

void EventSystem::unregisterEvent(EventType type, uint32_t id) {
	EventHandle& evh = eventsList[type];
	if (evh.callbacks.count(id)) evh.callbacks.erase(id);

	if (evh.callbacks.size() == 0) evh.registered = false;
}
*/
