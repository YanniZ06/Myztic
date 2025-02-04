#include "pch.h"

#include <Util.h>

std::map<const char*, IDGenerator*> IDGenerator::idTable;

// ResourceManager

Myztic::ResourceManager::ResourceManager() {
	busy.store(false);
	waiter = new std::binary_semaphore(0);
	requests.store(0);
}