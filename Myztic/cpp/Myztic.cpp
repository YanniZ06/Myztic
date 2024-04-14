// Myztic.cpp : Defines the functions for the static library.

#include "pch.h"
#include "framework.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "Myztic.h"
#include "glad/glad.h"
#include <iostream>

// Init sample window
void Myztic::fnMyztic()
{
	SDL_version ver;
	SDL_GetVersion(&ver);
	//std::cout << "Version: " << ver.major << "." << ver.minor << " Patch: " << ver.patch << std::endl;
	printf("Version: %u.%u Patch: %u\n", ver.major, ver.minor, ver.patch);

	SDL_Window* window = NULL;
	SDL_GLContext context;
	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) != false) {
		printf("Failed to init SDL\n");
		return;
	}

	window = SDL_CreateWindow("Myztic Engine Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, SDL_WINDOW_OPENGL);
	if (!window) {
		printf("Failed to create window\n");
	}

	context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, context);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	glViewport(0, 0, 680, 480);

	SDL_Event e;
	bool needToQuit = false;
	while (!needToQuit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) needToQuit = true;
			
		}
	}
}