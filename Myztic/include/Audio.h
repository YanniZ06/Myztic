#pragma once

class Audio {
public:
	static void initialize();
	static bool get_EXT_DeviceClock();
};

#include <audio/backend/ALExt.h>
inline bool Audio::get_EXT_DeviceClock() {
	return ALExt::deviceClockAvailable;
}
