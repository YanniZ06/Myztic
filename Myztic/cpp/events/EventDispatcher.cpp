#include "pch.h"

#include <events/EventDispatcher.h>

using namespace Myztic;

std::map<EventType, EventHandle> EventDispatcher::eventsList;

void EventDispatcher::unregisterEvent(EventType type, uint32_t id) {
	EventHandle& evh = eventsList[type];
	if (evh.callbacks.count(id)) evh.callbacks.erase(id);

	if (evh.callbacks.size() == 0) evh.registered = false;
}
