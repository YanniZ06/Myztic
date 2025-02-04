#pragma once

#include <string>
#include <alc.h>
#include <stdint.h>

namespace Myztic {
	class PlaybackDevice {
	public:
		std::string name;
		ALCdevice* handle;

		/// Obtains the current device clock value
		std::int64_t getDeviceClock();
	};
}
#include <audio/backend/ALExt.h>

inline std::int64_t Myztic::PlaybackDevice::getDeviceClock() {
	std::int64_t clockv = 0;
	ALExt::alcGetInteger64vSOFT(handle, 0x1600, 1, &clockv);
	return clockv;
}
