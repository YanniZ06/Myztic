#pragma once

#include <functional>
#include <map>
#include <alext.h>

namespace Myztic {
	class Application;
	class Model;

	enum EventType {
		EVENT_DEFAULT_AUDIO_DEVICE_CHANGED,
		EVENT_AUDIO_DEVICE_ADDED,
		EVENT_AUDIO_DEVICE_REMOVED,
		EVENT_MOUSEMOVE,
		EVENT_KEYBOARD,
		EVENT_POSITION_CHANGE,
		EVENT_SCALE_CHANGE,
		EVENT_ROTATION_CHANGE
	};

	struct EventHandle {
		bool registered;
		std::map<uint32_t, std::vector<std::function<void(void*)>>> callbacks;
		EventHandle(bool reg, std::map<uint32_t, std::vector<std::function<void(void*)>>> clb) : registered(reg), callbacks(clb) {}
		
		EventHandle() = default;
	};

	class EventDispatcher {
		friend Application;
		friend Model;
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
		static void registerEvent(EventType type, std::function<void(EventInfoStruct)> callbackFunc, uint32_t id/*, bool dupe = false*/) {
			EventHandle& evh = eventsList[type];
			evh.registered = true;

			// if (evh.callbacks.count(id) && !dupe) evh.callbacks.erase(id); // Dupe ID; unregister the event
			evh.callbacks[id].push_back([callbackFunc](void* data) {
				callbackFunc(*static_cast<EventInfoStruct*>(data));
			});
		};

		static void unregisterEvent(EventType type, uint32_t id);
	protected:
		// Map that tracks event types and whether their registered + what their callbacks are
		static std::map<EventType, EventHandle> eventsList;

		template<typename PhysicalEvent>
		static void dispatchBasedOnID(EventType type, PhysicalEvent inEvent, uint32_t inid) {
			EventHandle& evh = eventsList[type];
			if (!evh.registered) return;

			for (auto& callback : evh.callbacks[inid]) {
				callback(&inEvent);
			}
		}

		template<typename PhysicalEvent>
		static void dispatchEvent(EventType type, PhysicalEvent inEvent) {
			EventHandle& evh = eventsList[type];
			if (!evh.registered) return;

			for (auto& [id, callbacks] : evh.callbacks) {
				callbacks[0](&inEvent);
			}
		};
	};
}