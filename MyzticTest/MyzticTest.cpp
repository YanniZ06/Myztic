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
#include <graphics\Sprite.h>

class SceneB : Scene {
	virtual void load(Window* callerWindow) {
		std::cout << "Loaded to Window: " << (std::string)*callerWindow << "\n";
		std::cout << "ID: " << std::to_string(id) << "\n";
	}

	virtual void unload(Window* callerWindow) {
		std::cout << "Unloaded from Window: " << (std::string)*callerWindow << "\n";
	}

	virtual void enter() {
		std::cout << "SceneB entered\n";
		// Application::log_windows_cmd();
	}
	virtual void finish() {
		std::cout << "SceneB finished\n";
	}
};

class TestScene : Scene {
	void logLoaded() {
		size_t size = 0;
		Scene** loadedScenes = this->loadedWin->getLoadedScenes(size);
		std::cout << "Window::getLoadedScenes =>\n";

		int i = 0;
		while (i < size) {
			std::cout << "Scene Number "<< i <<":" << loadedScenes[i++]->id << "\n";
		}
		std::cout << "Finished\n";
	}

	virtual void load(Window* callerWindow) {
		std::cout << "Loaded to Window: " << callerWindow->name().c_str() << "\n";
	}
	virtual void enter() {
		Audio::initialize();

		Window* myzWin = Application::windows[this->loadedWin->id()];
		std::cout << myzWin->name() << "\n";
		myzWin->setName("WINDOW 1");

		std::cout << "Size of std::thread: " << sizeof(std::thread) << "\n";
		std::cout << "Size of std::thread* : " << sizeof(std::thread*) << "\n";


		logLoaded();
		Application::log_windows_cmd();

		// Segunda Windowa (excellente espanol)
		WindowParams paramsB = { "WINDOW 2", (Scene*)new SceneB(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
		Window* windowB = Window::create(paramsB);
		Application::log_windows_cmd();
		windowB->setX(windowB->x() + 250);

		myzWin->switchToContext(); //! THIS IS WHAT WAS MISSING BY THE WAY, SIMPLY THIS. GOD.
		
		// This batch of code should be in renderer actually, manual renderer handling is frowned upon for what we are doing but itll do to TEST
		//inputlayout is bound in Drawable.
		struct Vertex {
			glm::vec3 pos;
			glm::vec4 col;
			glm::vec2 uv;
		};
		std::vector<Vertex> vertices = {
			{{0.5, 0.5, 0.0}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5, -0.5, 0.0}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{-0.5, -0.5, 0.0 }, {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, 0.5, 0.0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};

		VertexBuffer buf(std::move(VertexLayout{}.Append(VertexLayout::Position3D).Append(VertexLayout::Float4Color).Append(VertexLayout::Texture2D)));
		for (int i = 0; i < vertices.size(); i++) {
			buf.EmplaceBack(vertices[i].pos, vertices[i].col, vertices[i].uv);
		}
		std::cout << buf.Size() << "\n";
		myzWin->renderer.drawables.push_back(Sprite(myzWin, buf, "assets/textures/yanni.png"));
	}

	virtual void update(float dt) {
		
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
	WindowParams p = { "Myztic Engine Test", (Scene*)scene, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
	Application::initMyztic(p, 60);
	
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