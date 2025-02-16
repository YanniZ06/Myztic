#include "pch.h"

#include <events/EventDispatcher.h>

using namespace Myztic;

std::map<EventType, EventHandle> EventDispatcher::eventsList;

template<typename EventInfoStruct>
void EventDispatcher::registerEvent(EventType type, std::function<void(EventInfoStruct)> callbackFunc, uint32_t id) {
	EventHandle& evh = eventsList[type];
	evh.registered = true;

	if (evh.callbacks.count(id)) evh.callbacks.erase(id); // Dupe ID; unregister the event
	evh.callbacks[id] = ((void*)&callbackFunc);
}

void EventDispatcher::unregisterEvent(EventType type, uint32_t id) {
	EventHandle& evh = eventsList[type];
	if (evh.callbacks.count(id)) evh.callbacks.erase(id);
}


template<typename PhysicalEvent, typename EventInfoStruct>
void EventDispatcher::dispatchEvent(EventType type, PhysicalEvent inEvent) {
	EventHandle& evh = eventsList[type];
	if (!evh.registered) return; 

	for (auto it = evh.callbacks.begin(); it != evh.callbacks.end(); ++it) {
		std::function<void(EventInfoStruct)>* cast_callback = static_cast<std::function<void(EventInfoStruct)>*>(it->second);
		(*cast_callback)(inEvent);
	}
}
