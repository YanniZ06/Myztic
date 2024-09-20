#include "pch.h" 

#include <display/WinThread.h>
#include <Application.h>

WinThread::WinThread(Window* winRef) {
	handle = std::thread(Application::start_winloop, winRef);
	signal = new std::binary_semaphore(0);
	parent = winRef;
}

void WinThread::deconstruct() {
	handle.join();
	delete signal;
}
