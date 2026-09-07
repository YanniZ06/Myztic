#pragma once

#include <functional>
#include <map>
#include <alext.h>

namespace Myztic {
	struct UserEventHandle {
		bool registered = false;
		std::map<uint32_t, std::vector<std::function<void(void*)>>> callbacks;
		UserEventHandle(bool reg, std::map<uint32_t, std::vector<std::function<void(void*)>>> clb) : registered(reg), callbacks(clb) {}

		UserEventHandle() = default;
	};

	class UserEventSystem {
	public:
		template<typename EventInfoStruct>
		/**
		 * Registers a callback for the passed in user-event.
		 *
		 * \param eventDefinition Name of the event-definition you want to add a callback for.
		 * \param callbackFunc A std::function representing a return type of void, taking in the info structure `EventInfoStruct` for the specified EventType;
		 * E.G if type is `EVENT_MOUSEMOVE`, then the template would require `MouseMoveEvent` for `EventInfoStruct`, which would then be passed in
		 * as the argument to the std::function ( `std::function<void(MouseMoveEvent)>` )
		 * \param id An unsigned integer id to identify this callback by.
		 * \note You can use the same id multiple times for the same EventType. Be aware that the callback can only be unregistered together with all other callbacks assigned under this id then.
		 */
		 void registerEvent(const char* eventDefinition, std::function<void(EventInfoStruct)> callbackFunc, uint32_t id);

		 void registerEventDefinition(const char* eventDefinition);

		 void removeDefinitionFromRegister(const char* eventDefinition);

		 void unregisterEvent(const char* eventDefinition, uint32_t id);

		template<typename PhysicalEvent>
		/**
		 * Dispatches the passed in user-event and calls all of its registered callbacks.
		 * 
		 * \param eventDefinition Name of the event-definition in the user-event-registry.
		 * \param inEvent
		 * \param id
		 */
		 void dispatchUserEvent(const char* eventDefinition, PhysicalEvent inEvent, uint32_t* id = nullptr);
	protected:
		// Map that tracks event types and whether their registered + what their callbacks are
		 std::map<const char*, UserEventHandle> eventsList;

		/*
		template<typename PhysicalEvent>
		 void dispatchBasedOnID(EventType type, PhysicalEvent inEvent, uint32_t inid) {
			EventHandle& evh = eventsList[type];
			if (!evh.registered) return;

			for (auto& callback : evh.callbacks[inid]) {
				callback(&inEvent);
			}
		}

		template<typename PhysicalEvent>
		 void dispatchEvent(EventType type, PhysicalEvent inEvent) {
			UserEventHandle& evh = eventsList[type];
			if (!evh.registered) return;

			for (auto& [id, callbacks] : evh.callbacks) {
				for (auto& callback : callbacks[id]) {
					callback(&inEvent);
				}
			}
		};
		*/
	};
}