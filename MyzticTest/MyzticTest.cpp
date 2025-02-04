#include <iostream>
//#include <Myztic.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <Application.h>
#include <SDL.h>

#include <display/Window.h>
#include <Scene.h>
#include <Audio.h>

// Graphics (remove when porting)
#include <graphics/backend/Shader.h>
#include <graphics/backend/ShaderInputLayout.h>

#include <thread>
#include <semaphore>

#include <graphics\Vertex.h>
#include <graphics\TexturedDrawable.h>
#include <graphics\Camera.h>

class SceneB : Myztic::Scene {
	virtual void load(Myztic::Window* callerWindow) {
		std::cout << "Loaded to Window: " << (std::string)*callerWindow << "\n";
		std::cout << "ID: " << std::to_string(id) << "\n";
	}

	virtual void unload(Myztic::Window* callerWindow) {
		std::cout << "Unloaded from Window: " << (std::string)*callerWindow << "\n";
	}

	virtual void enter() {
		std::cout << "SceneB entered\n";
		// Application::log_windows_cmd();
	}
	virtual void finish(Myztic::Scene* nextScene) {
		std::cout << "SceneB finished\n";
	}
};

class TestScene : Myztic::Scene {
	Myztic::TexturedDrawable* spr;
	float elapsed = 0.f;

	void logLoaded() {
		size_t size = 0;
		Scene** loadedScenes = this->loadedWin->getLoadedScenes(size);
		std::cout << "Window::getLoadedScenes =>\n";

		int i = 0;
		while (i < size) {
			std::cout << "Scene Number "<< i <<":" << loadedScenes[i++]->getId() << "\n";
		}
		std::cout << "Finished\n";
	}

	virtual void load(Myztic::Window* callerWindow) {
		std::cout << "Loaded to Window: " << callerWindow->name().c_str() << "\n";
	}
	virtual void enter() {
		Myztic::Audio::initialize();

		Myztic::Window* myzWin = Myztic::Application::windows[this->loadedWin->id()];
		std::cout << myzWin->name() << "\n";
		myzWin->setName("WINDOW 1");

		std::cout << "Size of std::thread: " << sizeof(std::thread) << "\n";
		std::cout << "Size of std::thread* : " << sizeof(std::thread*) << "\n";


		logLoaded();
		Myztic::Application::log_windows_cmd();

		// Segunda Windowa (excellente espanol)
		Myztic::WindowParams paramsB = { "WINDOW 2", (Scene*)new SceneB(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
		Myztic::Window* windowB = Myztic::Window::create(paramsB);
		Myztic::Application::log_windows_cmd();
		windowB->setX(windowB->x() + 250);

		myzWin->switchToContext(); //! THIS IS WHAT WAS MISSING BY THE WAY, SIMPLY THIS. GOD.
		
		// This batch of code should be in renderer actually, manual renderer handling is frowned upon for what we are doing but itll do to TEST
		//inputlayout is bound in Drawable.
		struct Vertex {
			glm::vec3 pos;
			glm::vec2 uv;
		};
		std::vector<Vertex> vertices = {
			{{-0.5, -0.5, -0.5},  {0.0, 0.0}},
			{{0.5, -0.5, -0.5},  {1.0, 0.0}},
			{{0.5,  0.5, -0.5},  {1.0, 1.0}},
			{{0.5,  0.5, -0.5},  {1.0, 1.0}},
			{{-0.5,  0.5, -0.5},  {0.0, 1.0}},
			{{-0.5, -0.5, -0.5},  {0.0, 0.0}},

			{{-0.5, -0.5,  0.5},  {0.0, 0.0}},
			{{0.5, -0.5,  0.5},  {1.0, 0.0}},
			{{0.5,  0.5,  0.5},  {1.0, 1.0}},
			{{0.5,  0.5,  0.5},  {1.0, 1.0}},
			{{-0.5,  0.5,  0.5},  {0.0, 1.0}},
			{{-0.5, -0.5,  0.5},  {0.0, 0.0}},

			{{-0.5,  0.5,  0.5},  {1.0, 0.0}},
			{{-0.5,  0.5, -0.5},  {1.0, 1.0}},
			{{-0.5, -0.5, -0.5},  {0.0, 1.0}},
			{{-0.5, -0.5, -0.5},  {0.0, 1.0}},
			{{-0.5, -0.5,  0.5},  {0.0, 0.0}},
			{{-0.5,  0.5,  0.5},  {1.0, 0.0}},

			{{0.5,  0.5,  0.5},  {1.0, 0.0}},
			{{0.5,  0.5, -0.5},  {1.0, 1.0}},
			{{0.5, -0.5, -0.5},  {0.0, 1.0}},
			{{0.5, -0.5, -0.5},  {0.0, 1.0}},
			{{0.5, -0.5,  0.5},  {0.0, 0.0}},
			{{0.5,  0.5,  0.5},  {1.0, 0.0}},

			{{-0.5, -0.5, -0.5},  {0.0, 1.0}},
			{{0.5, -0.5, -0.5},  {1.0, 1.0}},
			{{0.5, -0.5,  0.5},  {1.0, 0.0}},
			{{0.5, -0.5,  0.5},  {1.0, 0.0}},
			{{-0.5, -0.5,  0.5},  {0.0, 0.0}},
			{{-0.5, -0.5, -0.5},  {0.0, 1.0}},

			{{-0.5,  0.5, -0.5},  {0.0, 1.0}},
			{{0.5,  0.5, -0.5},  {1.0, 1.0}},
			{{0.5,  0.5,  0.5},  {1.0, 0.0}},
			{{0.5,  0.5,  0.5},  {1.0, 0.0}},
			{{-0.5,  0.5,  0.5},  {0.0, 0.0}},
			{{-0.5,  0.5, -0.5},  {0.0, 1.0}}
		};

		Myztic::VertexBuffer buf(std::move(Myztic::VertexLayout{}.Append(Myztic::VertexLayout::Position3D).Append(Myztic::VertexLayout::Texture2D)));
		for (int i = 0; i < vertices.size(); i++) {
			buf.EmplaceBack(vertices[i].pos, vertices[i].uv);
		}
		
		std::cout << buf.Size() << "\n";
		spr = new Myztic::TexturedDrawable(this, buf, "assets/textures/glint.png", false);
		spr->transformation = glm::rotate(glm::mat4(1.f), glm::radians(45.f), glm::vec3(1.f, 1.f, 1.f));
		mainCamera = new Myztic::Camera(Myztic::ProjectionType::Perspective, this, glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f));
		cameras.push_back(mainCamera);
		spr->camera = mainCamera;
		myzWin->renderer.drawables.push_back(spr);
	}

	virtual void update(float dt) {
		elapsed += dt;
		
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

	// SDL VERSION printing
	printf("Version: %u.%u Patch: %u\n", ver.major, ver.minor, ver.patch);


	//? Actual Myztic work
	TestScene* scene = new TestScene();
	Myztic::WindowParams p = { "Myztic Engine Test", (Myztic::Scene*)scene, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
	Myztic::Application::initMyztic(p, 60);
	
	// tMainTest();

#ifdef _DEBUG
	if (errHandle)
		fclose(errHandle);

	if (outputHandle)
		fclose(outputHandle);
	FreeConsole();
#endif
    return 0;
}