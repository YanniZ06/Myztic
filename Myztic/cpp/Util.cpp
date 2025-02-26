#include "pch.h"

#include <Util.h>

using namespace Myztic;


std::map<const char*, IDGenerator*> IDGenerator::idTable;

// ResourceManager

ResourceManager::ResourceManager() {
	busy.store(false);
	waiter = new std::binary_semaphore(0);
	requests.store(0);
}