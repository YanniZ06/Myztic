#include <iostream>
//#include <Myztic.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <Application.h>
#include <graphics/Texture2D.hpp>
#include <SDL.h>

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
#ifdef _DEBUG
	if (!AllocConsole())
		printf("COULD NOT ALLOCATE A CONSOLE");
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

	Application::initMyztic();

	Window* myzWin = Application::windows[1].get();
	std::cout << myzWin->name() << "\n";
	myzWin->setName("Myztic Main Window");

	Application::log_windows_cmd();

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