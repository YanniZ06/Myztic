#include "pch.h"

#include <string>
#include <iostream>

#include <stdexcept>

#include <audio\backend\ALExt.h>
#include <Audio.h>

using namespace Myztic;

std::vector<const char*> Audio::pbdList;
std::vector<const char*> Audio::micList;
SystemEvents Audio::systemEvents;
const char* Audio::defaultPbd;
const char* Audio::defaultMic;

void Audio::initialize()
{
	ALCdevice* tempDevice = alcOpenDevice(nullptr);
	ALCcontext* tempContext = alcCreateContext(tempDevice, nullptr);
	alcMakeContextCurrent(tempContext);

	std::string rawVersion = std::string(alGetString(AL_VERSION));
	rawVersion += "/"; // Terminating slash
	
	int cOffset = 0; // Character-loop offset
	int vOffset = 0; // versionNum offset
	bool driverObtained = false;
	std::string driverName = std::string("");
	std::string tempNumHolder = std::string("");

	int versionNums[3] = { 0, 0, 0 }; // MAJOR, MINOR, PATCH
	
	// This whole conglomerate of code is a bit fucking ugly, feel free to refactor in a way that's either performance around +- or faster (very few ms wont matter, otherwise leave as is)
	for (char c : rawVersion) {
		cOffset++;

		if (cOffset < 5) // Should be ALC Version + whitespace
			continue;

		// Add to driverName until we hit a space, indicating we're moving to the driverVersion
		if (!driverObtained && c != ' ') {
			driverName += c;
			continue;
		}
		else if(c == ' ') {
			driverObtained = true;
			continue;
		}

		// Add character to number-string unless its a period (or our terminating slash), in which case we convert the number and add it to the versionNums
		if (c != '.' && c != '/') {
			tempNumHolder += c;
		}
		else {
			versionNums[vOffset] = std::stoi(tempNumHolder);
			tempNumHolder = "";
			vOffset++;
		}
	}

	int MINOR_VER = versionNums[1];
	int PATCH_VER = versionNums[2];
	if (driverName != "ALSOFT" || MINOR_VER < 24 || (MINOR_VER == 24 && PATCH_VER < 2)) {
		std::string errMsg = std::string("Myztic Audio failed to initialize! You must have OpenAL Soft installed and use 1.24.2 minimum.\nDriver in use: " + driverName + "\nMINOR.PATCH: " + std::to_string(MINOR_VER) + "." + std::to_string(PATCH_VER));
		
		throw std::runtime_error(errMsg.c_str()); // todo: make crash function that displays a text window to the user
	}

	//std::string yes = std::string("Initialized Myztic Audio.\nDriver in use: " + driverName + "\nMINOR.PATCH: " + std::to_string(MINOR_VER) + "." + std::to_string(PATCH_VER) + "\n");
	//std::cout << yes.c_str();

	// OpenAL Soft version is supported, next fill the available devices lists
	// Statics already initialized???
	// pbdList = std::vector<const char*>();
	// micList = std::vector<const char*>();

	const char* deviceListRaw = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
	const char* in_deviceListRaw = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);

	while (strlen(deviceListRaw) > 0) {
		pbdList.push_back(deviceListRaw);
		deviceListRaw += strlen(deviceListRaw) + 1;
	}

	while (strlen(in_deviceListRaw) > 0) {
		micList.push_back(in_deviceListRaw);
		in_deviceListRaw += strlen(in_deviceListRaw) + 1;
	}
	defaultPbd = alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
	defaultMic = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);

	// Load in all extensions
	ALExt::initAllEXT(tempDevice);

	// Setup system audio events 
	Audio::systemEvents = SystemEvents(true);
	Audio::systemEvents.supported = std::map<const char*, bool>{ 
		// PlaybackDevice Events
		{"new_PlaybackDevice", ALExt::alcEventIsSupportedSOFT(ALC_EVENT_TYPE_DEVICE_ADDED_SOFT, ALC_PLAYBACK_DEVICE_SOFT) == 1},
		{"lost_PlaybackDevice", ALExt::alcEventIsSupportedSOFT(ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT, ALC_PLAYBACK_DEVICE_SOFT) == 1},
		{"changed_DefaultPlaybackDevice", ALExt::alcEventIsSupportedSOFT(ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT, ALC_PLAYBACK_DEVICE_SOFT) == 1},
		// Microphone Events
		{"new_Microphone", ALExt::alcEventIsSupportedSOFT(ALC_EVENT_TYPE_DEVICE_ADDED_SOFT, ALC_CAPTURE_DEVICE_SOFT) == 1},
		{"lost_Microphone", ALExt::alcEventIsSupportedSOFT(ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT, ALC_CAPTURE_DEVICE_SOFT) == 1},
		{"changed_DefaultMicrophone", ALExt::alcEventIsSupportedSOFT(ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT, ALC_CAPTURE_DEVICE_SOFT) == 1}
	};

	// basically finished 
	// todo: setup PlayBackDevice list aswell as InputDevice list (strings that can be passed into Audio:: functions to open them, to then open contexts on them or record or whatnot)

	// Close temporary context and device
	alcMakeContextCurrent(NULL);
	alcDestroyContext(tempContext);
	alcCloseDevice(tempDevice);
}

// Microphone

void Audio::refreshDefaultInputDevice()
{
	Audio::refreshInputDevices();
	Audio::defaultMic = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
}

void Audio::refreshInputDevices()
{
	micList.clear();
	const char* in_deviceListRaw = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);

	while (strlen(in_deviceListRaw) > 0) {
		micList.push_back(in_deviceListRaw);
		in_deviceListRaw += strlen(in_deviceListRaw) + 1;
	}
}

const char* Audio::getDefaultInputDevice()
{
	return Audio::defaultMic;
}

std::vector<const char*> Audio::getInputDevices() {
	return Audio::micList;
}


// Playback Device

void Audio::refreshDefaultPlaybackDevice() {
	Audio::refreshPlaybackDevices();
	Audio::defaultPbd = alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
}

void Audio::refreshPlaybackDevices()
{
	pbdList.clear();
	const char* deviceListRaw = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);

	while (strlen(deviceListRaw) > 0) {
		pbdList.push_back(deviceListRaw);
		deviceListRaw += strlen(deviceListRaw) + 1;
	}
}

const char* Audio::getDefaultPlaybackDevice() {
	return Audio::defaultPbd;
}

std::vector<const char*> Audio::getPlaybackDevices() {
	return Audio::pbdList;
}