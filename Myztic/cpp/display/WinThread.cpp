#include "pch.h" 

#include <display/WinThread.h>
#include <Application.h>

WinThread::WinThread(Window* winRef) {
	handle = new std::thread(Application::start_winloop, winRef);
	signal = new std::binary_semaphore(0);
	parent = winRef;
}

WinThread::~WinThread() {
	signal = nullptr;
	parent = nullptr;
}

void WinThread::destroy() {
	handle->join();
	delete signal;
	delete handle;
	// this->~WinThread();
}
