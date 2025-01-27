#pragma once

// Audio Events are more or less special, as they are not called by the EventTransmitter, but instead directly have their events transmitted when they're called
#include <stdint.h>
#include <utility>
#include <alc.h>

enum DeviceType {
	PLAYBACK_DEVICE = 0,
	MICROPHONE = 1
};

/**
 * The base for every audio event, seperated from BaseEvent to better suit its special purpose.
 * 
 * Use registerEvent 
 */
template<typename ...Args>
class BaseAudioEvent {
	friend class Application;
public:
	void registerEvent(void (*callbackFunction)(Args...));
	void registerEmpty();
	void unregisterEvent();

protected:
	BaseAudioEvent() = default;
	
	void (*callback)(Args...); // Should work as protected (?????)
	ALCenum eID = 0;
};

/**
 * Fires when a new audio device has become available.
 *
 *
 * Callback argument list for registerEvent():
 * * `const char * dName` -- name of the device that has become available
 * * `DeviceType dType` -- the `DeviceType` associated with this event
 *
 * \note Only to be used if `Audio::systemEvents.supported["new_PlaybackDevice" or "new_Microphone"]` returns true.
 */
class DeviceAddedEvent : public BaseAudioEvent<const char*, DeviceType> {
public:
	/**
	 * Should not be called, use Audio::systemEvents.onDeviceAdded instead.
	 */
	DeviceAddedEvent();
};

/**
 * Fires when an audio device is no longer available.
 *
 *
 * Callback argument list for registerEvent():
 * * `const char * dName` -- name of the device that has been lost
 * * `DeviceType dType` -- the `DeviceType` associated with this event
 *
 * \note Only to be used if `Audio::systemEvents.supported["lost_PlaybackDevice" or "lost_Microphone"]` returns true.
 */
class DeviceLostEvent : public BaseAudioEvent<const char*, DeviceType> {
public:
	/**
	 * Should not be called, use Audio::systemEvents.onDeviceLost instead.
	 */
	DeviceLostEvent();
};

/**
 * Fires when the default audio device changes.
 * 
 * 
 * Callback argument list for registerEvent():
 * * `const char * oldDefaultDevice` -- the default device name prior to the event;
 * * `const char * newDefaultDevice` -- name of the new default device
 * * `DeviceType dType` -- the `DeviceType` associated with this event
 * 
 * \note Only to be used if `Audio::systemEvents.supported["changed_DefaultPlaybackDevice" or "changed_DefaultMicrophone"]` returns true. 
 */
class DefaultDeviceChangedEvent : public BaseAudioEvent<const char*, const char*, DeviceType> {
public:
	/**
	 * Should not be called, use Audio::systemEvents.onDefaultDeviceChanged instead.
	 */
	DefaultDeviceChangedEvent();
};