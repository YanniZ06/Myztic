#pragma once
#include <imgui.h>

namespace Myztic {
	//should this be initialized as a unique pointer?
	class ImGuiHelper {
	public:
		/*
		* Constructor, returns ImGuiContext*
		* \param window The current window the ImGui should target
		* \param windowContext The context said window has active
		* \param flags ImGuiConfigFlags class (use the pipe operator to integrate between multiple flags)
		* \param initialized Bool containing data about whether or not ImGuiHelper intiialized or not
		*/
		ImGuiHelper(SDL_Window* window, void* windowContext, ImGuiConfigFlags flags, bool& initialized);
		//initalize new frame
		void InitializeFrame();
		//draw
		void Present();

		static void ProcessAll(SDL_Event* e);

		~ImGuiHelper();
		bool isInitialized = false;
	private:
		ImGuiContext* context;
		static std::vector<ImGuiContext*> contexts;
	};
};