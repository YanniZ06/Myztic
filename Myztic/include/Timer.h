#pragma once

#include <ctime>
#include <iostream>
#include <string>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

class Timer {
public:
	// Takes in a given ms-stamp and prints the difference between that and the current time in ms to the console
	// Optionally, you can name the operation in the opName parameter
	// For an equivalent in seconds use debugMeasure_sec
	inline static void debugMeasure(double timeStamp, const char* opName) {
		#ifdef DEBUG_MEASURES
		if (!opName) opName = "Unnamed";

		const double diff = ((double)clock() / (double)CLOCKS_PER_MS) - timeStamp;
		std::cout << "Debug Measure (\"" << opName << "\") Result: " << std::to_string(diff).c_str() << " MS Taken\n";
		#endif
		return;
	}

	// Behaves like debugMeasure but prints out a difference in seconds
	inline static void debugMeasure_sec(double timeStamp, const char* opName) {
		#ifdef DEBUG_MEASURES
		if (!opName) opName = "Unnamed";

		const double diff = ((double)clock() / (double)CLOCKS_PER_SEC) - timeStamp;
		std::cout << "Debug Measure (\"" << opName << "\") Result: " << std::to_string(diff).c_str() << " Seconds Taken\n";
		#endif
		return;
	}

	// Returns the ms elapsed since the program started
	// For a variant that returns a stamp in seconds unit use stamp_sec()
	inline static double stamp() {
		return (double)clock() / (double)CLOCKS_PER_MS;
	}

	// Returns the seconds elapsed since the program started
	inline static double stamp_sec() {
		return (double)clock() / (double)CLOCKS_PER_SEC;
	}
};

#undef CLOCKS_PER_MS // Remove if this is creating errors, it isn't necessary anyways