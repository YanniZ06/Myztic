#pragma once

#include <pch.h>

#include <utilities/Logger.h>

using namespace Myztic;

File Logger::file = File("./Log.txt", { Write, Truncate });

void Logger::logWarning(std::string toOutput, unsigned char warnLevel, const std::source_location location) { // todo: only log when config.warnLevel >= warnLevel
	auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ssstream;
	// First implementation, log to file later maybe also add a bool for logging to console additionally? or make that a config setting who
	char buf[256];
	::ctime_s(buf, 256, &current_time);
	buf[strcspn(buf, "\n")] = 0;
	ssstream << location.file_name() << '('
		<< location.line() << ':'
		<< location.column() << ") `"
		<< location.function_name() << "`: " << "Warning [" << warnLevel << "] ( " << buf << " ): " << toOutput << "\n";
	std::cout << ssstream.str();
	file.writeString(ssstream.str());
}
