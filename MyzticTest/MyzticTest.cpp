#include <iostream>
//#include <Myztic.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <events/Events.h>

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
#include <utilities\thread\ResourceManager.h>


using namespace Myztic;

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
	virtual void finish(Scene* nextScene) {
		std::cout << "SceneB finished\n";
	}
};

class TestScene : Scene {
	TexturedDrawable* spr;
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
					for (Camera* cam : win->getActiveScene()->cameras) {
						//to look left and right, we must modify the yaw axis by a certain variable according to the delta x (xrel) value
						glm::vec3 lookDirection = glm::vec3();
						glm::vec3 look_at = cam->get_look_at();
						cam->yawAngle += event.xRel;
						cam->pitchAngle = glm::clamp(cam->pitchAngle - event.yRel, -89.0f, 89.0f);
						lookDirection.x = cos(glm::radians(cam->yawAngle)) * cos(glm::radians(cam->pitchAngle));
						lookDirection.y = sin(glm::radians(cam->pitchAngle));
						lookDirection.z = sin(glm::radians(cam->yawAngle)) * cos(glm::radians(cam->pitchAngle));
						cam->set_look(glm::normalize(lookDirection));
					}
				}
			}
		};
		   
		EventDispatcher::registerEvent<MouseMoveEvent>(EVENT_MOUSEMOVE, std::function<void(MouseMoveEvent)>(mouseMoveCallback), 0);
		

		auto keyboardCallback = [&](KeyboardEvent event) {
			if (!event.keyDown) return;

			Window* win = event.focusWin;

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
					for (Camera* cam : win->getActiveScene()->cameras) {
						cam->set_position(cam->get_position() + (cam->get_look_at() * (Application::fps.getRawFrameTime() * 7.f)));
					}
				}
				break;
			case SDLK_s:
				if (win->getActiveScene() != nullptr && freeCamera) {
					for (Camera* cam : win->getActiveScene()->cameras) {
						cam->set_position(cam->get_position() - (cam->get_look_at() * (Application::fps.getRawFrameTime() * 7.f)));
					}
				}
				break;
			case SDLK_a:
				if (win->getActiveScene() != nullptr && freeCamera) {
					for (Camera* cam : win->getActiveScene()->cameras) {
						cam->set_position(cam->get_position() - (glm::normalize(glm::cross(cam->get_look_at(), Camera::UP)) * (Application::fps.getRawFrameTime() * 7.f)));
					}
				}
				break;
			case SDLK_d:
				if (win->getActiveScene() != nullptr && freeCamera) {
					for (Camera* cam : win->getActiveScene()->cameras) {
						cam->set_position(cam->get_position() + (glm::normalize(glm::cross(cam->get_look_at(), Camera::UP)) * (Application::fps.getRawFrameTime() * 7.f)));
					}
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

	//void mkThread()

	virtual void enter() {
		/*
		std::binary_semaphore s1 = std::binary_semaphore(0);
		std::binary_semaphore s2 = std::binary_semaphore(0);

		Sleep(5);

		std::thread thrd = std::thread([&s1, &s2]() {
			Sleep(1000);
			
			std::cout << "Freeing Lock 1\n";
			s1.release();

			Sleep(1000);

			std::cout << "Freeing Lock 1 again\n";
			s1.release();
		});
		thrd.detach();

		std::cout << "Aquiring Lock 1\n";
		s1.acquire(); // Lock this thread?
		std::cout << "Past Lock 1\n";

		std::cout << "Aquiring Lock 1 again\n";
		s1.acquire(); // Lock again ??
		std::cout << "Past Lock 1 again\n";
		*/

		/*
		ResourceManager x = ResourceManager();
		ResourceManager y = ResourceManager();

		std::thread thrd = std::thread([&x, &y]() {
			while (true) {

				if (x.isBusy()) {
					y.request();
					std::cout << "this passes!" << "\n";
					x.request();				
					std::cout << "requested already busy resourcemanager, this shouldn't run, right?" << "\n";
					break;
				}

				Sleep(1000);
			};
		});
		thrd.detach();

		x.request();
		Sleep(1100);
		y.request();
		//here, no code is progressed until we finish request from ANOTHER thread
		x.finishRequest();
		y.finishRequest();
		*/
		Audio::initialize();

		Window* myzWin = Application::windows[this->loadedWin->id()];
		std::cout << myzWin->name() << "\n";
		myzWin->setName("WINDOW 1");

		// logLoaded();
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

		VertexBuffer buf(std::move(VertexLayout{}.Append(VertexLayout::Position3D).Append(VertexLayout::Texture2D)));
		for (int i = 0; i < vertices.size(); i++) {
			buf.EmplaceBack(vertices[i].pos, vertices[i].uv);
		}
		
		std::cout << buf.Size() << "\n";
		spr = new TexturedDrawable(this, buf, "assets/textures/glint.png", false);
		spr->transformation = glm::rotate(glm::mat4(1.f), glm::radians(45.f), glm::vec3(1.f, 1.f, 1.f));
		mainCamera = new Camera(ProjectionType::Perspective, this, glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f));
		cameras.push_back(mainCamera);
		spr->camera = mainCamera;
		myzWin->renderer.drawables.push_back(spr);
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