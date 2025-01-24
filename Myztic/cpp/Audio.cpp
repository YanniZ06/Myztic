#include "pch.h"

#include <al.h>
#include <alc.h>
#include <string>
#include <iostream>

#include <stdexcept>


#include "Audio.h"

std::vector<const char*> Audio::pbdList;
std::vector<const char*> Audio::micList;

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

	pbdList = std::vector<const char*>();
	micList = std::vector<const char*>();

	const char* deviceList = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
	std::cout << deviceList << "\n";
	// todo: setup PlayBackDevice list aswell as InputDevice list (strings that can be passed into Audio:: functions to open them, to then open contexts on them or record or whatnot)

	// Close temporary context and device
	alcMakeContextCurrent(NULL);
	alcDestroyContext(tempContext);
	alcCloseDevice(tempDevice);
}
