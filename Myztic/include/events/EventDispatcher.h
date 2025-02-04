#pragma once

#include <events/MouseEvent.h>

#include <functional>
#include <map>

enum EventType {
	EVENT_AUDIO_DEVICE_ADDED,
	EVENT_AUDIO_DEVICE_REMOVED,
	EVENT_DEFAULT_AUDIO_DEVICE_CHANGED,
	EVENT_MOUSEMOVE
};

struct EventHandle {
	bool registered;
	std::vector<void*> callbacks;
	EventHandle(bool reg, std::vector<void*> clb) : registered(reg), callbacks(clb) {}
};

// Represents a pointer to a function with the signature void(MouseMoveEvent), obtained via &myMouseMoveEventCallbackFunction
typedef std::function<void(MouseMoveEvent)> MouseMoveEventCallback;

class EventDispatcher {
public:
	template<typename EventCallbackType>
	static void registerEvent(EventType type, EventCallbackType callbackFunc);
protected:
	// Map that tracks event types and whether their registered + what their callbacks are
	static std::map<EventType, EventHandle> eventsList;

	// EventCallbackType is necessary for our static cast to receive
	template<typename PhysicalEvent, typename EventCallbackType>
	static void dispatchEvent(EventType type, PhysicalEvent inEvent);

};
