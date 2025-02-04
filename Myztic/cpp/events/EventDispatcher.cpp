#include "pch.h"

#include <events/EventDispatcher.h>

std::map<EventType, EventHandle> EventDispatcher::eventsList;

template<typename EventCallbackType>
void EventDispatcher::registerEvent(EventType type, EventCallbackType callbackFunc) {
	EventHandle& evh = eventsList[type];
	evh.registered = true;
	evh.callbacks.push_back((void*)&callbackFunc);
}

template<typename PhysicalEvent, typename EventCallbackType>
void EventDispatcher::dispatchEvent(EventType type, PhysicalEvent inEvent) {
	EventHandle& evh = eventsList[type];
	if (!evh.registered) return;

	for (void* callback : evh.callbacks.begin()) {
		EventCallbackType* cast_callback = static_cast<EventCallbackType*>(callback);
		(*cast_callback)(inEvent);
	}
}
