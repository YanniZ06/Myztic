#include <pch.h>

#include <audio\backend\ALExt.h>
#include <events/AudioEvent.h>

template <typename ...Args>
void BaseAudioEvent<Args...>::registerEvent(void (*callbackFunction)(Args...)) {
	callback = callbackFunction;
	ALExt::alcEventControlSOFT(1, &eID, 1);
}

template<typename ...Args>
void BaseAudioEvent<Args...>::registerEmpty() {
	auto empty = [](Args...) {}; // Create Lambda as function substitute

	callback = empty;
	ALExt::alcEventControlSOFT(1, &eID, 1);
}


template <typename ...Args>
void BaseAudioEvent<Args...>::unregisterEvent() {
	callback = nullptr;
	ALExt::alcEventControlSOFT(1, &eID, 0);
};

DeviceAddedEvent::DeviceAddedEvent() : BaseAudioEvent<const char*, DeviceType>() {
	eID = ALC_EVENT_TYPE_DEVICE_ADDED_SOFT;
}

DeviceLostEvent::DeviceLostEvent() : BaseAudioEvent<const char*, DeviceType>() {
	eID = ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT;
}

DefaultDeviceChangedEvent::DefaultDeviceChangedEvent() : BaseAudioEvent<const char*, const char*, DeviceType>() {
	eID = ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT;
}