#pragma once

#include <events/MouseEvent.h>

#include <functional>
#include <map>

namespace Myztic {
	enum EventType {
		EVENT_AUDIO_DEVICE_ADDED,
		EVENT_AUDIO_DEVICE_REMOVED,
		EVENT_DEFAULT_AUDIO_DEVICE_CHANGED,
		EVENT_MOUSEMOVE
	};

	struct EventHandle {
		bool registered;
		std::map<uint32_t, void*> callbacks;
		EventHandle(bool reg, std::map<uint32_t, void*> clb) : registered(reg), callbacks(clb) {}

		EventHandle() = default;
	};

	class EventDispatcher {
	public:
		template<typename EventInfoStruct>
		/**
		 * Registers a callback for the specified event.
		 * 
		 * \param type `EventType` you want to add a callback for.
		 * \param callbackFunc A std::function representing a return type of void, taking in the info structure `EventInfoStruct` for the specified EventType; 
		 * E.G if type is `EVENT_MOUSEMOVE`, then the template would require `MouseMoveEvent` for `EventInfoStruct`, which would then be passed in
		 * as the argument to the std::function ( `std::function<void(MouseMoveEvent)>` )
		 * \param id An unsigned integer id to identify this callback by. If the id is already in use by another event of type `type`, the old callback is unregistered and overwritten by this one.
		 */
		static void registerEvent(EventType type, std::function<void(EventInfoStruct)> callbackFunc, uint32_t id);

		static void unregisterEvent(EventType type, uint32_t id);
	protected:
		// Map that tracks event types and whether their registered + what their callbacks are
		static std::map<EventType, EventHandle> eventsList;

		// EventInfoStruct is necessary for our static cast to receive
		template<typename PhysicalEvent, typename EventInfoStruct>
		static void dispatchEvent(EventType type, PhysicalEvent inEvent);

	};
}