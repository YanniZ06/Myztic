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

#include <events/Events.h>

// Graphics (remove when porting)
#include <graphics/backend/Shader.h>
#include <graphics/backend/ShaderInputLayout.h>
#include <graphics/Drawable.h>
#include <graphics\TexturedDrawable.h>

#include <thread>
#include <semaphore>

#include <graphics\Vertex.h>
#include <graphics\primitives\Quad.h>
#include <graphics\primitives\Line.h>
#include <graphics\primitives\Cube.h>
#include <graphics\Camera.h>
#include <utilities\thread\ResourceManager.h>
#include <graphics\Mesh.h>
#include <graphics\Model.h>
#include <filesys/FileSystem.h>

using namespace Myztic;

class SceneB : Scene {
	Model* model;
	virtual void load(Window* callerWindow) {
		std::cout << "Loaded to Window: " << (std::string)*callerWindow << "\n";
		std::cout << "ID: " << std::to_string(id) << "\n";
	}

	virtual void unload(Window* callerWindow) {
		std::cout << "Unloaded from Window: " << (std::string)*callerWindow << "\n";
	}

	virtual void enter() {
		Window* myzWin = Application::windows[this->loadedWin->id()];
		std::cout << myzWin->name() << "\n";
		myzWin->setName("WINDOW 2");

		myzWin->switchToContext();
		
		mainCamera = new Camera(ProjectionType::Perspective, this, glm::vec3(0.f, 0.2f, 3.f), glm::vec3(0.f, 0.f, -1.f));
		cameras.push_back(mainCamera);

		auto vs = Shader(GL_VERTEX_SHADER, "vs.glsl");

		auto fs = Shader(GL_FRAGMENT_SHADER, "fs.glsl");

		std::vector<Shader> shaders = { vs, fs };

		model = new Model("backpack.obj", this, mainCamera, shaders);
		model->pushToRenderer();

		/*
			to implement raycasting:
			make a line/ray equation from the camera/mouse forward
			let planes that are part of the boundingbox be able to form an equation based on th existing 3d coordinates, this happens by subtracting some of the points, getting the normal
			to the plane and such
		*/
		//model->set_rotation(glm::vec3(glm::half_pi<float>(), 0.f, 0.f));

		//printf(FileSystem::readDirectory("./assets")[0].c_str());
	}
	virtual void finish(Scene* nextScene) {
		std::cout << "SceneB finished\n";
	}
};

class TestScene : Scene {
	//TexturedDrawable* spr;
	Line* line;
	float elapsed = 0.f;

	bool freeCamera = false;

	void logLoaded() {
		size_t size = 0;
		Scene** loadedScenes = this->loadedWin->getLoadedScenes(size);
		std::cout << "Window::getLoadedScenes =>\n";

		int i = 0;
		while (i < size) {
			std::cout << "Scene Number "<< i <<":" << loadedScenes[i++]->getId() << "\n";
		}
		std::cout << "Finished\n";

		auto mouseMoveCallback = [&](MouseMoveEvent event) {
			Window* win = event.focusWin;
			if (win) {
				if (win->getActiveScene() != nullptr && freeCamera) {
					Camera* mainCam = win->getActiveScene()->mainCamera;
					//to look left and right, we must modify the yaw axis by a certain variable according to the delta x (xrel) value
					glm::vec3 lookDirection = glm::vec3();
					glm::vec3 look_at = mainCam->get_look_at();
					mainCam->yawAngle += event.xRel;
					mainCam->pitchAngle = glm::clamp(mainCam->pitchAngle - event.yRel, -89.0f, 89.0f);
					lookDirection.x = cos(glm::radians(mainCam->yawAngle)) * cos(glm::radians(mainCam->pitchAngle));
					lookDirection.y = sin(glm::radians(mainCam->pitchAngle));
					lookDirection.z = sin(glm::radians(mainCam->yawAngle)) * cos(glm::radians(mainCam->pitchAngle));
					mainCam->set_look(glm::normalize(lookDirection));
				}

				if (win->getActiveScene() != nullptr && !freeCamera) {
					
				}
			}
		};
		   
		EventDispatcher::registerEvent<MouseMoveEvent>(EVENT_MOUSEMOVE, std::function<void(MouseMoveEvent)>(mouseMoveCallback), 0);
		

		auto keyboardCallback = [&](KeyboardEvent event) {
			if (!event.keyDown) return;

			Window* win = event.focusWin;
			if (win == nullptr) return; // IMGUI Window / Non Focused but still registered press, Ignore for now

			Camera* mainCam = win->getActiveScene()->mainCamera;
			switch (event.key) {
			case SDLK_INSERT:
				freeCamera = !freeCamera;
				SDL_CaptureMouse(freeCamera ? SDL_TRUE : SDL_FALSE);
				SDL_SetRelativeMouseMode(freeCamera ? SDL_TRUE : SDL_FALSE);
				break;
				//this is fucking horrible i should find another way to do this
				// i made it a BIT better for you <3 -yanni (ps you can make a map for the rough calculations later :)
			case SDLK_w:
				if (win->getActiveScene() != nullptr && freeCamera) {
					mainCam->set_position(mainCam->get_position() + (mainCam->get_look_at() * (Application::fps.getRawFrameTime() * 7.f)));
				}
				break;
			case SDLK_s:
				if (win->getActiveScene() != nullptr && freeCamera) {
					mainCam->set_position(mainCam->get_position() - (mainCam->get_look_at() * (Application::fps.getRawFrameTime() * 7.f)));
				}
				break;
			case SDLK_a:
				if (win->getActiveScene() != nullptr && freeCamera) {
					mainCam->set_position(mainCam->get_position() - (glm::normalize(glm::cross(mainCam->get_look_at(), Camera::UP)) * (Application::fps.getRawFrameTime() * 7.f)));
				}
				break;
			case SDLK_d:
				if (win->getActiveScene() != nullptr && freeCamera) {
					mainCam->set_position(mainCam->get_position() + (glm::normalize(glm::cross(mainCam->get_look_at(), Camera::UP)) * (Application::fps.getRawFrameTime() * 7.f)));
				}
				break;
			default: break;
			}
		};

		EventDispatcher::registerEvent<KeyboardEvent>(EVENT_KEYBOARD, std::function<void(KeyboardEvent)>(keyboardCallback), 0);
	}

	virtual void load(Window* callerWindow) {
		std::cout << "Loaded to Window: " << callerWindow->name().c_str() << "\n";
	}

	virtual void enter() {
		ResourceManager x = ResourceManager();
		ResourceManager y = ResourceManager();

		std::thread thrd = std::thread([&x, &y]() {
			std::cout << "requested x on thread 2..." << "\n";
			x.request();
			std::cout << "x on thread 2!" << "\n";


			Sleep(1500);
			x.finishRequest();
			std::cout << "finished x on thread 2!" << "\n";

			Sleep(500);

			std::cout << "requested y on thread 2..." << "\n";
			y.request();
			std::cout << "y on thread 2!" << "\n";

			y.finishRequest();
			std::cout << "finished y on thread 2!" << "\n";

		});
		thrd.detach();

		std::thread thrd2 = std::thread([&x, &y]() {
			Sleep(50);
			std::cout << "requested x on thread 3..." << "\n";
			x.request();
			std::cout << "x on thread 3!" << "\n";

			Sleep(50);
			x.finishRequest();
			std::cout << "finished x on thread 3!" << "\n";
		});

		thrd2.detach();
		Sleep(200);
		std::cout << "requested x on thread 1..." << "\n";
		x.request();
		std::cout << "x on thread 1!" << "\n";

		std::cout << "requested y on thread 1..." << "\n";
		y.request();
		std::cout << "y on thread 1!" << "\n";

		Sleep(600);

		//here, no code is progressed until we finish request from ANOTHER thread
		x.finishRequest();
		std::cout << "finished x on thread 1!" << "\n";

		y.finishRequest();
		std::cout << "finished y on thread 1!" << "\n";

		Audio::initialize();

		Window* myzWin = Application::windows[this->loadedWin->id()];
		std::cout << myzWin->name() << "\n";
		myzWin->setName("WINDOW 1");

		Application::log_windows_cmd();

		// Segunda Windowa (excellente espanol)
		WindowParams paramsB = { "WINDOW 2", (Scene*)new SceneB(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
		Window* windowB = Window::create(paramsB);
		Application::log_windows_cmd();
		windowB->setX(windowB->x() + 250);
		windowB->initialize_imgui();

		myzWin->switchToContext(); //! THIS IS WHAT WAS MISSING BY THE WAY, SIMPLY THIS. GOD.

		std::cout << "Size of std::thread: " << sizeof(std::thread) << "\n";
		std::cout << "Size of std::thread* : " << sizeof(std::thread*) << "\n";

		logLoaded();
		Application::log_windows_cmd();
		
		Line* line = Line::createLine(this, glm::vec2(100, 0.0), glm::vec2(100, 200), glm::vec4(0.2, 0.5, 0.2, 1.0));

		/*this works, it projects objects onto the camera(controls are inverted for some reason here); do note that when rendering onto an orthographic plane when you want to
		* exclusively use 2d objects, that 3d plane rules still apply; therefore, rotating would fuck up the view and the image will be distorted (if not invisible)
		* if you wish to use orthographic planes in pair with 3d objects; don't forget to scale them up because the projection is scaled according to the window
		* (children of the camera must be scaled on resize as to refit the camera presumably? if on a perspective projection, it seems as if recreating the projection
		* fixes the issue.)
		*/
		mainCamera = new Camera(ProjectionType::Orthographic, this, glm::vec3(-200.f, -50.f, 1.f), glm::vec3(0.f, 0.f, -1.f), -1000.f, 1000.f);
		cameras.push_back(mainCamera);

		line->camera = mainCamera;
		myzWin->renderer.drawables.push_back(line);

		Line* ray = Line::createRay(this, glm::vec3(0.0, 0.0, 0.0), glm::vec3(300.0, 300.0, 300.0));
		ray->camera = mainCamera;
		myzWin->renderer.drawables.push_back(ray);

		//the previous is an example of pairing a purely 2d line with a 3d object and casting them onto an orthographic plane; both of them exist in "3d space" because
		//they're being viewed from above.
		//you can exclude the camera from being affected by transformations or view changes by not adding it to the cameras array
	}

	virtual void update(float dt) {
		elapsed += dt;
		//std::cout << "FPS: " << Application::fps.used() << "\n";
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
	Application::initMyztic(p, 120); // 120 / the fps is currently not representative of the actual fps used (uncapped or monitor refresh-rate, depending on os settings and such)
	
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