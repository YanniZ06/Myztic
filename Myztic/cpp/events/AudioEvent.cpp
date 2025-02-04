#include <pch.h>

#include <audio\backend\ALExt.h>
#include <events/AudioEvent.h>

void Myztic::BaseAudioEvent::registerEvent(void (*callbackFunction)(void*)) {
	callback = callbackFunction;
	ALExt::alcEventControlSOFT(1, &eID, 1);
}

void Myztic::BaseAudioEvent::registerEmpty() {
	auto empty = [](void*) {}; // Create Lambda as function substitute

	callback = empty;
	ALExt::alcEventControlSOFT(1, &eID, 1);
}


void Myztic::BaseAudioEvent::unregisterEvent() {
	callback = nullptr;
	ALExt::alcEventControlSOFT(1, &eID, 0);
}
Myztic::BaseAudioEvent::BaseAudioEvent() {
	dName = "";
	dType = INVALID;
};

Myztic::DeviceAddedEvent::DeviceAddedEvent() : BaseAudioEvent() {
	eID = ALC_EVENT_TYPE_DEVICE_ADDED_SOFT;
}

Myztic::DeviceLostEvent::DeviceLostEvent() : BaseAudioEvent() {
	eID = ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT;
}

Myztic::DefaultDeviceChangedEvent::DefaultDeviceChangedEvent() : BaseAudioEvent() {
	eID = ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT;
	newDefaultDevice = "";
}