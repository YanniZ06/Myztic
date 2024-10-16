#pragma once

#include <thread>
#include <semaphore>
#include <utility>

class Window;

class WinThread {
public:
	WinThread() = default;
	WinThread(Window* winRef);
	~WinThread();

	void destroy();

	std::thread* handle; // We cannot copy a std::thread directly, so we reference it instead
	std::binary_semaphore* signal;
	Window* parent;
};