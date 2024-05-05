#include <iostream>
//#include <Myztic.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <Application.h>
#include <graphics/Texture2D.hpp>
#include <SDL.h>
#include <Scene.h>

#include <thread>
#include <semaphore>
/*
std::binary_semaphore signal1{ 0 };
std::binary_semaphore signal2{ 0 };
std::binary_semaphore signal3{ 0 };
void t2() {
	signal2.acquire();
	std::cout << "Started thread 2 (2 seconds)\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "Finished thread 2\n";
}

void t1() {
	signal1.acquire();
	std::cout << "Started thread 1 (1 second)\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Finished thread 1\n";
}

void tMainTest() {
	std::thread tr1(t1);
	std::thread tr2(t2);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	signal1.release();
	signal2.release();
	std::cout << "Released 2 threads\n";

	tr1.join();
	tr2.join();
	std::cout << "Finished Main Thread\n";
}
*/

class TestScene : Scene {
	virtual void load(Window* callerWindow) {
		Window* myzWin = Application::windows[1].get();
		std::cout << myzWin->name() << "\n";
		myzWin->setName("Myztic Main Window");

		Application::log_windows_cmd();
	}
};

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
#ifdef _DEBUG
	if (!AllocConsole())
		printf("COULD NOT ALLOCATE A CONSOLE\n");
	HRESULT hr = GetLastError();
	if (FAILED(hr))
		return -1;

	AttachConsole(GetCurrentProcessId());
	hr = GetLastError();
	if (FAILED(hr))
		return -1;

	//redirect console prints
	FILE* outputHandle;
	freopen_s(&outputHandle, "conout$", "w", stdout);
	FILE* errHandle;
	freopen_s(&errHandle, "conerr$", "w", stderr);
#endif

	SDL_version ver;
	SDL_GetVersion(&ver);

	printf("Version: %u.%u Patch: %u\n", ver.major, ver.minor, ver.patch);

	TestScene* scene = new TestScene();
	Application::initMyztic((Scene*) scene);

	// tMainTest();

	SDL_Event e;
	bool needToQuit = false;
	while (!needToQuit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) needToQuit = true;
		}

		SDL_Delay(1);
	}

#ifdef _DEBUG
	if (errHandle)
		fclose(errHandle);

	if (outputHandle)
		fclose(outputHandle);
	FreeConsole();
#endif
    return 0;
}