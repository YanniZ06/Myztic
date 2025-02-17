#pragma once

#include <map>
#include <al.h>
#include <alc.h>

namespace Myztic {
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
		 * if(Audio::systemEvents.supported["new_PlaybackDevice"]) 
		 * {
		 *		auto callbackFunc = [](DeviceAddedEvent event) {
		 *			const char* dType = event.isCapture ? "Microphone" : "Playback Device";
		 *			std::cout << "New " << dType << " was added: " << event.dName << "\n";
		 *		};
		 * 
		 *		EventDispatcher::registerEvent<DeviceAddedEvent>(EVENT_AUDIO_DEVICE_ADDED, std::function<void(DeviceAddedEvent)>(callbackFunc), 0);
		 * }
		 * ```
		 * \sa EventDispatcher::registerEvent
		 * \sa toggleEventWatch
		 */
		std::map<const char*, bool> supported;

		/**
		 * Toggles listening for the input audio event. 
		 * Whether the event can actually be listened for is determined by the `supported` map.
		 * 
		 * \note The actual callback function needs to be registered via the EventDispatcher.
		 * 
		 * \param eventType EventType::EVENT_DEFAULT_AUDIO_DEVICE_CHANGED, EventType::EVENT_AUDIO_DEVICE_ADDED or EventType::EVENT_AUDIO_DEVICE_REMOVED. Any other input will cause undefined behavior.
		 * \param toggle True if the event should be listened for, otherwise false.
		 */
		void toggleEventWatch(int eventType, bool toggle);
	protected:
		static int OPENAL_SYSTEM_EVENT_SDLEVENTTYPE;
		static void ALC_APIENTRY openalEventCallback(ALCenum eventType, ALCenum deviceType, ALCdevice* device, ALCsizei length, const ALCchar* message, void* userParam) AL_API_NOEXCEPT17;
	private:
		/// Basically holds references to SDLEvent data1, to deal with the fact we only have 2 user arguments
		/// worked around by using event.user.code
		//std::map<const char*, std::tuple<const char*, DeviceType>> eventDataContainer;
		
	};
}
