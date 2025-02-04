#pragma once

#include <vector>
#include <audio/backend/SystemEvents.h>

class PlaybackDevice;

enum PlaybackDeviceError {
	/// The passed in string does not match any available device on the system
	INVALID_DEVICE_NAME,
	/// The backend failed to move the old device content into the new device, the old device will instead stay active	(if available)
	REOPEN_DEVICE_FAILED
};

class Audio {
	friend class Application;
public:
	static void initialize();
	/**
	 * Returns a vector with all available audio output devices.
	 * 
	 * \warning A DeviceAddedEvent and DeviceLostEvent must be registered over Audio::systemEvents for this list to update automatically.
	 * If you do not wish to use a callback you may use the registerEmpty() functions.
	 * 
	 * \warning If Audio::systemEvents.supported["new_PlaybackDevice" or "lost_PlaybackDevice"] yield false, the available playback devices
	 * must be manually updated using Audio::refreshPlaybackDevices()
	 */
	static std::vector<const char*> getPlaybackDevices();

	/**
	 * Returns the default playback device.
	 * 
	 * \warning A DefaultDeviceChangedEvent must be registered over Audio::systemEvents for this list to update automatically.
	 * If you do not wish to use a callback you may use the registerEmpty() functions.
	 * 
	 * \warning If Audio::systemEvents.supported["changed_DefaultPlaybackDevice"] yields false, the default playback device
	 * must be manually updated using Audio::refreshDefaultPlaybackDevice()
	 */
	static const char* getDefaultPlaybackDevice();

	/**
	 * Checks if new playback devices have been added or if ones were lost and updates accordingly.
	 */
	static void refreshPlaybackDevices();

	/**
	 * Checks for a change in the default playback device and updates accordingly.
	 *
	 * \remark This function automatically calls Audio::refreshPlaybackDevices()
	 */
	static void refreshDefaultPlaybackDevice();

	/**
	 * Opens a new output device for audio playback.
	 * 
	 * \remark There is no functionality to directly "close" a PlaybackDevice, as each call to this function
	 * closes the old PlaybackDevice on its own.
	 * 
	 * \param deviceName A valid name listed in getPlaybackDevices(), or NULL for the default available device.
	 * \throw std::runtime_error where what() describes one of the  
	 */
	static PlaybackDevice* openPlaybackDevice(const char* deviceName);

	static PlaybackDevice* currentPlaybackDevice;
	/**
	 * Returns a vector with all available audio input devices.
	 * 
	 * \warning A DeviceAddedEvent and DeviceLostEvent must be registered over Audio::systemEvents for this list to update automatically.
	 * If you do not wish to use a callback you may use the registerEmpty() functions.
	 * 
	 * \warning If Audio::systemEvents.supported["new_Microphone" or "lost_Microphone"] yield false, the available microphones
	 * must be manually updated using Audio::refreshInputDevices()
	 */
	static std::vector<const char*> getInputDevices();

	/**
	 * Returns the default microphone.
	 * 
	 * \warning A DefaultDeviceChangedEvent must be registered over Audio::systemEvents for this list to update automatically.
	 * If you do not wish to use a callback you may use onDeviceAdded.registerEmpty().
	 * 
	 * \warning If Audio::systemEvents.supported["changed_DefaultMicrophone"] yields false, the default microphone
	 * must be manually updated using Audio::refreshDefaultInputDevice()
	 */
	static const char* getDefaultInputDevice();

	/**
	 * Checks if new microphones have been added or if ones were lost and updates accordingly.
	 */
	static void refreshInputDevices();

	/**
	 * Checks for a change in the default microphone and updates accordingly.
	 *
	 * \remark This function automatically calls Audio::refreshInputDevices()
	 */
	static void refreshDefaultInputDevice();

	static AUD::SystemEvents systemEvents;
protected:
	/// List of available PlaybackDevice's
	static std::vector<const char*> pbdList;

	static const char* defaultPbd;

	/// List of available Microphone's
	static std::vector<const char*> micList;

	static const char* defaultMic;
};