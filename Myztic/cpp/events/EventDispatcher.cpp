#include "pch.h"

#include <events/EventDispatcher.h>

std::map<Myztic::EventType, Myztic::EventHandle> Myztic::EventDispatcher::eventsList;

template<typename EventCallbackType>
void Myztic::EventDispatcher::registerEvent(EventType type, EventCallbackType callbackFunc) {
	EventHandle& evh = eventsList[type];
	evh.registered = true;
	evh.callbacks.push_back((void*)&callbackFunc);
}

template<typename PhysicalEvent, typename EventCallbackType>
void Myztic::EventDispatcher::dispatchEvent(EventType type, PhysicalEvent inEvent) {
	EventHandle& evh = eventsList[type];
	if (!evh.registered) return;

	for (void* callback : evh.callbacks.begin()) {
		EventCallbackType* cast_callback = static_cast<EventCallbackType*>(callback);
		(*cast_callback)(inEvent);
	}
}
