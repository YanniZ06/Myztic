#pragma once

#include <thread>
#include <semaphore>
#include <utility>

class Window;

class WinThread {
public:
	WinThread() = default;
	WinThread(Window* winRef);

	void destroy();

	std::thread handle;
	std::binary_semaphore* signal;
	Window* parent;
};