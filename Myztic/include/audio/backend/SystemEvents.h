#pragma once

#include <map>
#include <events/AudioEvent.h>
#include <al.h>
// #include <tuple>

namespace AUD { // Might remove later, currently this is a precaution for an additional SystemEvents class showing up at some point
	class SystemEvents {
		friend class Application;
	public:
		SystemEvents() = default;
		SystemEvents(bool active); // SystemEvents being a static is auto-initialized by cpp, great! Could turn that off but just in case id rather have a seperate constructor. Love it.

		/**
		 * A map of audio-related system events.
		 * 
		 * The event-type structure goes as follows:
		 * 
		 * "[eventType]_[deviceType]"
		 * 
		 * Where eventType may be:
		 * * new -- `device of type [deviceType] is now available`.
		 * * lost -- `device of type [deviceType] is no longer available`.
		 * 
		 * And deviceType may be:
		 * * PlaybackDevice
		 * * Microphone
		 * 
		 * Additionally, you may query for the events "changed_DefaultPlaybackDevice" and "changed_DefaultMicrophone",
		 * however as of OpenAL Soft 1.24.2, these events are supported on every audio backend that has any system event support for OpenAL.
		 * 
		 * \remark
		 * The supported system events are generally pre-defined, for example Windows applications that are not built on UWP (which by standard they are not) support all event types,
		 * as long as WasAPI (Windows personal audio backend) is used.
		 * 
		 * \remark
		 * Linux applications using the PipeWire or PulseAudio backends also offer support for all events.
		 * 
		 * \remark
		 * Apple-based applications using CoreAudio (Apples personal audio backend) only support default-audio-device change events.
		 * 
		 * \remark
		 * As of implementation time there are no other audio backends that offer event support for OpenAL.
		 * 
		 * ### Example Usage
		 * ```cpp
		 * void callback(const char* msg, DeviceType type) {
		 *		std::cout << "[NEW OPENALSOFT DEVICE] " << msg << "\n";
		 * }
		 * 
		 * if(Audio::systemEvents.supported["new_PlaybackDevice"]) {
		 *		Audio::systemEvents.onDeviceAdded.registerEvent(&callback);
		 * }
		 * ```
		 * \sa onDeviceAdded
		 * \sa onDeviceLost
		 * \sa onDefaultDeviceChanged
		 */
		std::map<const char*, bool> supported;

		DeviceAddedEvent onDeviceAdded;
		DefaultDeviceChangedEvent onDefaultDeviceChanged;

	protected:
		static int OPENAL_SYSTEM_EVENT_SDLEVENTTYPE;
		static void ALC_APIENTRY openalEventCallback(ALCenum eventType, ALCenum deviceType, ALCdevice* device, ALCsizei length, const ALCchar* message, void* userParam) AL_API_NOEXCEPT17;
	private:
		/// Basically holds references to SDLEvent data1, to deal with the fact we only have 2 user arguments
		/// worked around by using event.user.code
		//std::map<const char*, std::tuple<const char*, DeviceType>> eventDataContainer;
		
	};
}
