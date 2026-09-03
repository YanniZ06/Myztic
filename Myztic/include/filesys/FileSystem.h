#pragma once
#include <filesystem>

namespace Myztic {
	class FileSystem {
	public:
		static std::string absolutePath(std::string relative);
		static bool createDirectory(std::string path);
		static bool deleteDirectory(std::string directory);
		static bool deleteFile(std::string path);
		static bool pathExists(std::string path);
		static std::string fullPath(std::string relativePath);
		static bool isDirectory(std::string path);
		static std::vector<std::string> readDirectory(std::string path);
		static void rename(std::string path, std::string newPath);
		static std::filesystem::file_status status(std::string path);
		static unsigned int getFileSize(std::string path);
		static std::filesystem::file_time_type getLastEditTime(std::string path);
		static tm getTimeInfo(std::string path);
		static std::string getCurrentDirectory();
	};
}
