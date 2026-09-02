#pragma once
#include <vector>

namespace Myztic {
	enum FileMode {
		Binary = std::ios::binary,
		Read = std::ios::in,
		Write = std::ios::out,
		Start = std::ios::ate,
		Append = std::ios::app,
		Truncate = std::ios::trunc
	};

	class File {
	public:
		//Opens a file handle for I/O operations
		File(std::string filePath, std::vector<FileMode> modes);

		void writeFromBytes(void* data, unsigned int size);
		//reads the entire file into a string
		void readInto(std::string& string);
		void readSize(void* fill, unsigned int length);
		void readCharStar(char* arr, unsigned int length);
		void seek(int offset, std::ios::seekdir position);
		void writeString(std::string str);

		void flush();
		void openFile(std::string filePath, std::vector<FileMode> modes);
		void close();
		void clear();
		bool isOpen() const;
		std::streampos getLength() const;

	private:
		std::fstream fileHandle;
		unsigned long long fileSize;
	};
}