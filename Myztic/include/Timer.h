#pragma once

#include <ctime>
#include <iostream>
#include <string>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

namespace Myztic {
class Timer {
	public:
		/**
		 * Takes in a given millisecond stamp and prints the difference between that and the current time in ms to the console.
		 *
		 * \param timeStamp Stamp taken at an earlier time to measure with
		 * \param opName Optionally used to name the measurement for a clearer console log
		 * \see debugMeasure_sec
		 */
		inline static void debugMeasure(double timeStamp, const char* opName) {
#ifdef DEBUG_MEASURES
			if (!opName) opName = "Unnamed";

			const double diff = ((double)clock() / (double)CLOCKS_PER_MS) - timeStamp;
			std::cout << "Debug Measure (\"" << opName << "\") Result: " << std::to_string(diff).c_str() << " MS Taken\n";
#endif
			return;
		}

		/**
		 * Takes in a given stamp in seconds and prints the difference between that and the current time in seconds to the console.
		 *
		 * \param timeStamp Stamp taken at an earlier time to measure with
		 * \param opName Optionally used to name the measurement for a clearer console log
		 * \see debugMeasure
		 */
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
}

#undef CLOCKS_PER_MS // Remove if this is creating errors, it isn't necessary anyways