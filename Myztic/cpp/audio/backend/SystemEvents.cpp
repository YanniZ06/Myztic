#include "pch.h"

#include <al.h>
#include <audio\backend\SystemEvents.h>
#include <iostream>
#include <SDL.h>
#include <audio/backend/ALExt.h>
#include <events/EventDispatcher.h>

using namespace Myztic;

int SystemEvents::OPENAL_SYSTEM_EVENT_SDLEVENTTYPE;
// todo: ?
void ALC_APIENTRY SystemEvents::openalEventCallback(ALCenum eventType, ALCenum deviceType, ALCdevice* device, ALCsizei length, const ALCchar* message, void* userParam) AL_API_NOEXCEPT17 {
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = deviceType;
	// event.user.timestamp = SDL_GetTicks();
	//const char* 
	event.user.data1 = (void*)eventType;
	event.user.data2 = (void*)message;
	event.user.type = OPENAL_SYSTEM_EVENT_SDLEVENTTYPE;

	
	// Thread-safe alternative to SDL_PushEvent, as this code may be called on a different thread from openal
	SDL_PeepEvents(&event, 1, SDL_ADDEVENT, SDL_USEREVENT, SDL_USEREVENT);
}
SystemEvents::SystemEvents(bool active) {
	OPENAL_SYSTEM_EVENT_SDLEVENTTYPE = SDL_RegisterEvents(1);

	ALExt::alcEventCallbackSOFT(openalEventCallback, nullptr);

}
std::map<int, int> myzToALEvent = std::map<int, int>{
	{EVENT_DEFAULT_AUDIO_DEVICE_CHANGED, ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT},
	{EVENT_AUDIO_DEVICE_ADDED, ALC_EVENT_TYPE_DEVICE_ADDED_SOFT},
	{EVENT_AUDIO_DEVICE_REMOVED, ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT}
};

void Myztic::SystemEvents::toggleEventWatch(int eventType, bool toggle) {
	ALExt::alcEventControlSOFT(1, &myzToALEvent[eventType], toggle ? 1 : 0); // todo: test this
}
