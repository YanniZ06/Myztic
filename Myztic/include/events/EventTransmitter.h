#pragma once

#include <events/BaseEvent.h>
#include <map>

class EventTransmitter {
public:
	static std::map<int, EventTypeBase> assignedEvents;
};

constexpr int EVENT_AUDIO_DEVICE_ADDED = 0;
constexpr int EVENT_AUDIO_DEVICE_REMOVED = 1;
constexpr int EVENT_DEFAULT_AUDIO_DEVICE_CHANGED = 2;
