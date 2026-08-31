#include <pch.h>
#include <utilities\thread\ResourceManager.h>

using namespace Myztic;

ResourceManager::ResourceManager() {
	if (!hasStaticInstance.load()) {
		threadMapWaiter = new std::binary_semaphore(1);
		hasStaticInstance.store(true);
	}
}

std::binary_semaphore* ResourceManager::threadMapWaiter;
std::map<std::thread::id, std::vector<ResourceManager*>> ResourceManager::threadResources;
std::atomic_bool ResourceManager::hasStaticInstance;
/*
std::map<std::thread::id, std::vector<ResourceManager*>> threadResources;
std::map<std::thread::id, std::vector<std::condition_variable*>> waitingThreads;
std::mutex globalMutex;
*/