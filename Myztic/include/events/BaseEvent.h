#pragma once

#include <stdint.h>
#include <utility>

class EventTypeBase {
};

template<typename ...Args>
class BaseEvent : EventTypeBase {
	friend class EventTransmitter;
	friend class Application;
public:
	BaseEvent() = default;

	inline BaseEvent registerEvent(void (*callbackFunction)(Args...)) {
		callback = callbackFunction;
		if (onRegister) onRegister();

		return *this;
	}

	inline void unregisterEvent() {
		// todo
	};

	void (*callback)(Args...); // Should work as protected????
protected:
	virtual void onRegister();


	// todo: make this work and not force personal callback call
	/*
	inline void transmitEvent(Args&&... fArgs) {
		(*callback)(std::forward<Args>(fArgs));
	}
	*/
};


