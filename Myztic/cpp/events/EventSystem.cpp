#include "pch.h"

#include <events/EventSystem.h>

using namespace Myztic;

std::map<EventType, EventHandle> EventSystem::eventsList;

template<typename EventInfoStruct>
void EventSystem::registerEvent(EventType type, std::function<void(EventInfoStruct)> callbackFunc, uint32_t id/*, bool dupe = false*/) {
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
