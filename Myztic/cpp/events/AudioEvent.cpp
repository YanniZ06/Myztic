#include <pch.h>

#include <audio\backend\ALExt.h>
#include <events/AudioEvent.h>

void BaseAudioEvent::registerEvent(void (*callbackFunction)(void*)) {
	callback = callbackFunction;
	ALExt::alcEventControlSOFT(1, &eID, 1);
}

void BaseAudioEvent::registerEmpty() {
	auto empty = [](void*) {}; // Create Lambda as function substitute

	callback = empty;
	ALExt::alcEventControlSOFT(1, &eID, 1);
}


void BaseAudioEvent::unregisterEvent() {
	callback = nullptr;
	ALExt::alcEventControlSOFT(1, &eID, 0);
}
BaseAudioEvent::BaseAudioEvent() {
	dName = "";
	dType = INVALID;
};

DeviceAddedEvent::DeviceAddedEvent() : BaseAudioEvent() {
	eID = ALC_EVENT_TYPE_DEVICE_ADDED_SOFT;
}

DeviceLostEvent::DeviceLostEvent() : BaseAudioEvent() {
	eID = ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT;
}

DefaultDeviceChangedEvent::DefaultDeviceChangedEvent() : BaseAudioEvent() {
	eID = ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT;
	newDefaultDevice = "";
}