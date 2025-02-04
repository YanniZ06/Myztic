#pragma once

#include <string>
#include <al.h>
#include <alc.h>
#include <list>

using std::list;
namespace Myztic {
	class Microphone {
		std::string name;
		ALCdevice* handle;
	};
}