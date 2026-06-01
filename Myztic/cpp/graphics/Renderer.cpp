#include "pch.h" 

#include <display/Window.h>
#include <Application.h>
#include <iostream>
#include <graphics/Drawable.h>
#include <graphics/PrecompiledShaders.h>
#include <graphics/Renderer.h>

using namespace Myztic;

Renderer::Renderer(Window* drawTarget) {
	targetWin = drawTarget;

	pendingRequests = std::vector<RenderRequestBase>();
	nextRequests = std::vector<RenderRequestBase>();
}

Renderer::~Renderer() {
	// pendingRequests.clear();
	// nextRequests.clear();
	targetWin = nullptr;
}

//the following is a set of utility functions for common shapes utilizable by continuously calling them pre-draw (queues the draw in the scene and then the request gets popped, making it so you need to queue it again on the next draw)
void Renderer::drawLine(glm::vec2 p1, glm::vec2 p2)
{
	DLineRequest<glm::vec2> req(p1, p2);

	pendingRequests.push_back(req.base());
}

void Renderer::drawRay(glm::vec3 p1, glm::vec3 p2)
{
	DLineRequest<glm::vec3> req(p1, p2);

	pendingRequests.push_back(req.base());
}

// This is the full renderer logic per Window that is used in Application's render phase

void Renderer::startRender() {
	if (!targetWin->focused() && !targetWin->renderOutOfFocus) {
		pendingRequests.clear(); //? Maybe not a smart idea, gotta make sure we have sort of 2 render queues, especially for the double buffering shittamajig -- check later, double buffering is just fine with this
		return;
	}
	if (SDL_GL_MakeCurrent(targetWin->handle, targetWin->context) != 0) {
		std::cout << "whoopsie daisy, couldn't make current opengl context to window: " << targetWin->name().c_str() << "\n";
		return;
	}

	if (targetWin->imgui_initialized) {
		ImGui::SetCurrentContext(targetWin->imgui_context);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
			//ImGuiDockNodeFlags_PassthruCentralNode is NECESSARY
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		bool demo = true;
		ImGui::ShowDemoWindow(&demo);
		//ImGui::Begin("demo2");
		//ImGui::Text("Lorem ipsum dolor");
		//ImGui::End();
	}

	//ImGui::End();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//CHECK_GL(glViewport(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y));
	glClearColor(0.7f, 0.2f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//? probably rewrite this so it's more dynamically written depending on the scene? (scene has a draw function and consequently, the scene draws its own drawables (if
	//? scene is active graphically or requires to be precached or yadda yadda)
	for (Drawable* d : drawables) {
		d->prepareDraw();
		if (!d->usesEBO) {
			CHECK_GL(glDrawArrays(d->vert_type, 0, d->vertexData.Size()));
		}
		else {
			CHECK_GL(glDrawElements(d->vert_type, (GLsizei)d->vert_indices.size(), GL_UNSIGNED_INT, NULL));
		}
		d->finishDraw();
	}

	//take the entire vector backwards (recently queued gets drawn first)
	/*for (size_t i = pendingRequests.size(); i > 0; i--) {
		//must decrement by 1
		RenderRequestBase request = pendingRequests[i - 1];

		//process this entry
		switch (request.type()) {
		case D_LINE: {
			//static cast into our type
			DLineRequest<glm::vec2>& cast = static_cast<DLineRequest<glm::vec2>&>(request);

			//unfortunately; the way we handle this isn't reflective (C++ has no reflection; therefore, the necessity of the switch-case)
			VertexBuffer buffet = VertexBuffer(std::move(VertexLayout{}.Append(VertexLayout::Position2D)));
			VBO vbo = VBO::make();

			vbo.bind();
			vbo.fill((void*)buffet.GetData(), buffet.SizeBytes(), GL_STATIC_DRAW);

			ShaderInputLayout shaderLayout = ShaderInputLayout(buffet.GetLayout().GetDescription());
			shaderLayout.setVertexLayout();
			shaderLayout.enableAllAttribs();

			ShaderProgram program = ShaderProgram();
			program.attach(PrecompiledShaders::line_vs);
			program.attach(PrecompiledShaders::line_fs);

			program.link();
			program.bind();

			shaderLayout.bindInputLayout();

			glDrawArrays(GL_LINES, 0, 2);
			program.unbind();
			shaderLayout.unbind();
			vbo.unbind();

			break;
		}
		default:
			//unimplemented
			break;
		}

		//pop the last element (remove it and continue the loop)
		pendingRequests.pop_back();
	}*/
	
	glDisable(GL_BLEND);
	//glDisable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
}

void Renderer::endRender() {
	glBindVertexArray(0); 

	if (targetWin->imgui_initialized) {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(targetWin->handle, targetWin->context);
		}
	}

	//swap the buffers (double-buffering in action for this window.)
	SDL_GL_SwapWindow(targetWin->handle);
}