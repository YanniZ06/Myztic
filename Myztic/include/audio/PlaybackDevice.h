#pragma once

#include <string>
#include <alc.h>
#include <stdint.h>

class PlayBackDevice {
public:
	std::string name;
	ALCdevice* handle;

	/// Obtains the current device clock value, only available if Audio::get_EXT_DeviceClock returns true
	std::int64_t getDeviceClock();
};

#include <audio/backend/ALExt.h>

inline std::int64_t PlayBackDevice::getDeviceClock() {
	std::int64_t clockv = 0;
	ALExt::alcGetInteger64vSOFT(handle, 0x1600, 1, &clockv);
	return clockv;
}
