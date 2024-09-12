#pragma once

#include <thread>
#include <semaphore>
#include <utility>

class Window;

class WinThread {
public:
	WinThread() = default;
	WinThread(Window* winRef);

	std::thread handle;
	std::binary_semaphore* signal;
	Window* parent;
};