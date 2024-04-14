#include <iostream>
#include <Myztic.h>
#include <Application.h>
#include <SDL.h>

int WinMain()
{
	SDL_version ver;
	SDL_GetVersion(&ver);
	//std::cout << "Version: " << ver.major << "." << ver.minor << " Patch: " << ver.patch << std::endl;
	printf("Version: %u.%u Patch: %u\n", ver.major, ver.minor, ver.patch);

    Application::initMyztic();

	SDL_Event e;
	bool needToQuit = false;
	while (!needToQuit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) needToQuit = true;

		}
		//Application::windows[1];

		// printf("hello\n");
		SDL_Delay(1);
	}

	// Application::windows[1]->~Window();
	// printf("You arent supposed to HAPPEN YET\n");

    return 0;
}