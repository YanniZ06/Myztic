#pragma once

#include <string>
#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <time.h>
#include <chrono>
#include <source_location>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesys\File.h>

namespace Myztic {
	class Logger {
	public:
		// Same as below but for general info like generating windows or sprites or whatever, this can go however deep technically but we SHOULDNT overdo it past testing!!!
		static void logInfo(std::string toOutput, unsigned char debugLevel, const std::source_location location = std::source_location::current());
		/**
		 *
		 * @param toOutput String output to log.
		 * @param warnLevel Required warn level to output this warning, 0 is reserved for WARNINGS_OFF do not use
		 * @param location Caller Location, do not modify.
		 * .
		 */
		static void logWarning(std::string toOutput, unsigned char warnLevel, const std::source_location location = std::source_location::current());

	private:
		static File file;
	};
}