#pragma once

#include <vector>
#include <audio/backend/SystemEvents.h>

class Audio {
public:
	static void initialize();
	/**
	 * Returns a vector with all available audio output devices.
	 * 
	 * If Audio::systemEvents.supported["new_PlaybackDevice"] yields true
	 */
	static std::vector<const char*> getPlaybackDevices();

	static const char* getDefaultPlaybackDevice();

	static void refreshDefaultPlaybackDevice();
	/**
	 * Returns a vector with all available audio input devices.
	 */
	static std::vector<const char*> getInputDevices();

	static AUD::SystemEvents systemEvents;
private:
	/// List of available PlaybackDevice's
	static std::vector<const char*> pbdList;

	static const char* defaultPbd;

	/// List of available Microphone's
	static std::vector<const char*> micList;
};