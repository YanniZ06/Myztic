#include <pch.h>
#include <filesys\FileSystem.h>
#include <sys/stat.h>

using namespace Myztic;
using namespace std;

string FileSystem::absolutePath(string relative)
{
    return filesystem::absolute(relative).generic_string();
}

bool FileSystem::createDirectory(string path)
{
    return filesystem::create_directory(path);
}

bool FileSystem::deleteDirectory(string directory)
{
    if (filesystem::is_directory(directory)) {
        return filesystem::remove(directory);
    }

    return false;
}

bool FileSystem::deleteFile(string path)
{
    if (filesystem::is_regular_file(path))
        return filesystem::remove(path);

    return false;
}

bool FileSystem::pathExists(string path)
{
    return filesystem::exists(path);
}

string FileSystem::fullPath(string relativePath)
{
    return getCurrentDirectory() + relativePath;
}

bool FileSystem::isDirectory(string path)
{
    return filesystem::is_directory(path);
}

vector<string> FileSystem::readDirectory(string path)
{
    vector<string> ret;

    for (const auto& entry : filesystem::directory_iterator(path)) {
        ret.push_back(entry.path().generic_string());
    }

    return ret;
}

void FileSystem::rename(string path, string newPath)
{
    filesystem::rename(path, newPath);
}

std::filesystem::file_status Myztic::FileSystem::status(std::string path)
{
    return std::filesystem::status(path);
}

unsigned int FileSystem::getFileSize(string path)
{
    return std::filesystem::file_size(path);
}

filesystem::file_time_type Myztic::FileSystem::getLastEditTime(std::string path)
{
    return filesystem::last_write_time(path);
}

tm Myztic::FileSystem::getTimeInfo(std::string path)
{
    struct stat t_stat;
    stat(path.c_str(), &t_stat);
    struct tm timeinfo;
    localtime_s(&timeinfo, &t_stat.st_ctime);
    
    return timeinfo;
}

string FileSystem::getCurrentDirectory() {
    return filesystem::current_path().generic_string();
}