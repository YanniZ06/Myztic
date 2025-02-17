#pragma once

// Audio Events are more or less special, as they are not called by the EventTransmitter, but instead directly have their events transmitted when they're called
#include <stdint.h>
#include <utility>
#include <alc.h>

namespace Myztic {
	/**
	 * Fires when a new audio device has become available.
	 * 
	 * \note Only to be used if `Audio::systemEvents.supported["new_PlaybackDevice" or "new_Microphone"]` returns true.
	 * \note Use Audio::systemEvents.toggleEventWatch to ensure the event is being listened for.
	 */
	struct DeviceAddedEvent {
		/// The name of the device that has become available
		const char* dName;
		/// The device type associated with this event, true if Microphone, otherwise PlaybackDevice
		bool isCapture;

		DeviceAddedEvent(const char* dN, bool cap) :dName(dN), isCapture(cap) {}
	};

	/**
	 * Fires when an audio device is no longer available.
	 * 
	 * \note Only to be used if `Audio::systemEvents.supported["lost_PlaybackDevice" or "lost_Microphone"]` returns true.
	 * \note Use Audio::systemEvents.toggleEventWatch to ensure the event is being listened for.
	 */
	struct DeviceLostEvent {
		/// The name of the device that has been lost
		const char* dName;
		/// The device type associated with this event, true if Microphone, otherwise PlaybackDevice
		bool isCapture;

		DeviceLostEvent(const char* dN, bool cap) :dName(dN), isCapture(cap) {}
	};

	/**
	 * Fires when the default audio device changes.
	 * 
	 * \note Only to be used if `Audio::systemEvents.supported["changed_DefaultPlaybackDevice" or "changed_DefaultMicrophone"]` returns true.
	 * \note Use Audio::systemEvents.toggleEventWatch to ensure the event is being listened for.
	 */
	struct DefaultDeviceChangedEvent {
		/// The default device name prior to the event
		const char* old_DName;
		/// The name of the new default device
		const char* new_DName;
		/// The device type associated with this event, true if Microphone, otherwise PlaybackDevice
		bool isCapture;

		DefaultDeviceChangedEvent(const char* old_dN, const char* new_dN, bool cap) :old_DName(old_dN), new_DName(new_dN), isCapture(cap) {}
	};
}