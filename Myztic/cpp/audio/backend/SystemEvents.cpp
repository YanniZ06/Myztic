#include "pch.h"

#include <al.h>
#include <audio\backend\SystemEvents.h>
#include <iostream>
#include <SDL.h>
#include <audio/backend/ALExt.h>

int AUD::SystemEvents::OPENAL_SYSTEM_EVENT_SDLEVENTTYPE;
// todo: ?
void ALC_APIENTRY AUD::SystemEvents::openalEventCallback(ALCenum eventType, ALCenum deviceType, ALCdevice* device, ALCsizei length, const ALCchar* message, void* userParam) AL_API_NOEXCEPT17 {
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = (deviceType == ALC_PLAYBACK_DEVICE_SOFT) ? PLAYBACK_DEVICE : MICROPHONE;
	// event.user.timestamp = SDL_GetTicks();
	//const char* 
	event.user.data1 = (void*)eventType;
	event.user.data2 = (void*)message;
	event.user.type = OPENAL_SYSTEM_EVENT_SDLEVENTTYPE;

	
	// Thread-safe alternative to SDL_PushEvent, as this code may be called on a different thread from openal
	SDL_PeepEvents(&event, 1, SDL_ADDEVENT, SDL_USEREVENT, SDL_USEREVENT);
}
AUD::SystemEvents::SystemEvents(bool active)
{
	onDeviceAdded = DeviceAddedEvent();
	onDefaultDeviceChanged = DefaultDeviceChangedEvent();

	OPENAL_SYSTEM_EVENT_SDLEVENTTYPE = SDL_RegisterEvents(1);

	// todo: finish later, leave out for now
	ALExt::alcEventCallbackSOFT(openalEventCallback, nullptr);

}