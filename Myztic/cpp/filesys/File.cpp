#include <pch.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <format>
#include <filesys\File.h>

using namespace Myztic;
using namespace std;

File::File(std::string filePath, std::vector<FileMode> modes)
{
	ios::openmode mode = modes[0];
	for (int i = 1; i < modes.size(); i++)
		mode = mode | modes[i];

	this->fileHandle = fstream(filePath, mode);

	if (!fileHandle.is_open()) {
		fileHandle.close();
		throw std::format("[[EXCEPTION]]; FILE FAILED TO OPEN FROM PATH {}", filePath);
	}

	fileHandle.seekg(0, std::ios::end);
	this->fileSize = fileHandle.tellg();
	fileHandle.seekg(0, std::ios::beg);
}

void File::writeFromBytes(void* data, unsigned int size)
{
	if (!fileHandle.write(reinterpret_cast<char*>(data), size)) printf("error occurred writing from bytes\n");
		fileHandle.flush();
}

void File::writeString(std::string str) {
	fileHandle << str;
	fileHandle.flush();
}

void File::seek(int offset, std::ios::seekdir position) {
	if (!fileHandle.seekg(offset, position)) printf("error seeking\n");
}

void File::readSize(void* into, unsigned int length) {
	if (!fileHandle.read(reinterpret_cast<char*>(into), length)) printf("error occurred reading sized\n");
}

void File::readCharStar(char* charStar, unsigned int length) {
	if (!fileHandle.read(charStar, length)) printf("error occurred reading char star\n");
}

void File::readInto(std::string& str)
{
	std::string line;
	std::stringstream ss;

	while (std::getline(fileHandle, line)) {
		ss << line;
	}

	str = ss.str();
}

void File::close() {
	fileHandle.close();
}

void File::clear() {
	fileHandle.clear();
}

void File::flush() {
	fileHandle.flush();
}

void File::openFile(std::string filePath, std::vector<FileMode> modes) {
	if (fileHandle)
		fileHandle.close();

	ios::openmode mode = modes[0];
	for (int i = 1; i < modes.size(); i++)
		mode = mode | modes[i];

	this->fileHandle = fstream(filePath, mode);

	if (!fileHandle.is_open()) {
		fileHandle.close();
		throw std::format("[[EXCEPTION]]; FILE FAILED TO OPEN FROM PATH {}", filePath);
	}

	fileHandle.seekg(0, std::ios::end);
	this->fileSize = fileHandle.tellg();
	fileHandle.seekg(0, std::ios::beg);
}

bool File::isOpen() const {
	return fileHandle.is_open();
}

std::streampos File::getLength() const {
	return fileSize;
}