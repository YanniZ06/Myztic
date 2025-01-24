#pragma once

#include <vector>
class Audio {
public:
	static void initialize();
	/**
	 * Returns a vector with all available audio output devices.
	 */
	static std::vector<const char*> getPlaybackDevices();
	/**
	 * Returns a vector with all available audio input devices.
	 */
	static std::vector<const char*> getInputDevices();
private:
	/// List of available PlaybackDevice's
	static std::vector<const char*> pbdList;

	/// List of available Microphone's
	static std::vector<const char*> micList;
};

inline std::vector<const char*> Audio::getPlaybackDevices() {
	return Audio::pbdList;
}

inline std::vector<const char*> Audio::getInputDevices() {
	return Audio::micList;
}
