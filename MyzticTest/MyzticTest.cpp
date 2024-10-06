#include <iostream>
//#include <Myztic.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <Application.h>
#include <SDL.h>
#include <Scene.h>

#include <thread>
#include <semaphore>

#include <graphics\Drawable.h>

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

Drawable* triangle = nullptr;

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
		Window* myzWin = Application::windows[this->loadedWin->id()].get();
		std::cout << myzWin->name() << "\n";
		myzWin->setName("WINDOW 1");

		logLoaded();
		Application::log_windows_cmd();

		// Segunda Windowa (excellente espanol)
		WindowParams paramsB = { "WINDOW 2", (Scene*)new SceneB(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
		Window* windowB = Window::create(paramsB);
		Application::log_windows_cmd();
		windowB->setX(windowB->x() + 250);

		myzWin->switchToContext(); //! THIS IS WHAT WAS MISSING BY THE WAY, SIMPLY THIS. GOD.

		std::vector<InputProperty> vec = std::vector<InputProperty>();
		vec.push_back(ShaderInputLayout::POSITION);

		triangle = new Drawable(this->loadedWin, vec);
		triangle->vbo.bind();

		GLfloat vertices[9] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};

		triangle->vbo.fill(vertices, 9, GL_STATIC_DRAW);

		triangle->inputLayout.enableAllAttribs();
		triangle->vbo.unbind();

		Shader vs = Shader(GL_VERTEX_SHADER, "assets/shaders/vs.glsl");
		Shader fs = Shader(GL_FRAGMENT_SHADER, "assets/shaders/fs.glsl");
		triangle->shaderProgram.attach(vs);
		triangle->shaderProgram.attach(fs);
		triangle->shaderProgram.link();
		vs.deleteShader();
		fs.deleteShader();

		this->loadedWin->renderer.drawables.push_back(triangle);

		//SceneB* nScene = new SceneB();

		//std::cout << Application::windows[2].get()->loadScene((Scene*)nScene) << "\n";
		//std::cout << this->loadedWin->loadScene((Scene*)nScene) << "\n";
		//logLoaded();

		//std::cout << Application::windows[2].get()->unloadScene((Scene*)nScene) << "\n";
		//std::cout << this->loadedWin->loadScene((Scene*)nScene) << "\n";
		//logLoaded();
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

	printf("Version: %u.%u Patch: %u\n", ver.major, ver.minor, ver.patch);

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