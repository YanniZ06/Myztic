#include "pch.h"
#include <SDL.h>
#include <vector>
#include <display\ImGuiHelper.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

std::vector<ImGuiContext*> Myztic::ImGuiHelper::contexts;

Myztic::ImGuiHelper::ImGuiHelper(SDL_Window* window, void* windowContext, ImGuiConfigFlags flags, bool& initialized){
	this->context = ImGui::CreateContext();
	ImGui::SetCurrentContext(this->context);

	ImGuiIO& imgui_iosys = ImGui::GetIO(); (void)imgui_iosys;
	imgui_iosys.ConfigFlags = flags;
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	imgui_iosys.ConfigDpiScaleFonts = true;
	imgui_iosys.ConfigDpiScaleViewports = true;

	if (!ImGui_ImplSDL2_InitForOpenGL(window, windowContext)) {
		printf("Failed to initialize ImGui impl for SDL2 initialized for OpenGL, deinitializing all imgui systems.\n");
		ImGui::DestroyContext();
		initialized = false;
		this->isInitialized = false;
		return;
	}
	else
		ImGui_ImplOpenGL3_Init("#version 330 core");

	initialized = true;
	contexts.push_back(context);
	this->isInitialized = true;
}

void Myztic::ImGuiHelper::InitializeFrame(){
	if (!isInitialized) return;
	ImGui::SetCurrentContext(this->context);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
		//ImGuiDockNodeFlags_PassthruCentralNode is NECESSARY
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	
}

void Myztic::ImGuiHelper::Present()
{
	if (!isInitialized) return;
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}

void Myztic::ImGuiHelper::ProcessAll(SDL_Event* e) {
	//if (!isInitialized) return;
	for (int i = 0; i < contexts.size(); i++) {
		ImGui::SetCurrentContext(contexts[i]);
		ImGui_ImplSDL2_ProcessEvent(e);
	}
}

Myztic::ImGuiHelper::~ImGuiHelper(){
	if (!isInitialized) return;
	isInitialized = false;
	ImGui::SetCurrentContext(context);
	contexts.erase(std::remove(contexts.begin(),contexts.end(), context), contexts.end());
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}