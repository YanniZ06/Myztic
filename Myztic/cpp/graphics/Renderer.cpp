#include "pch.h" 

#include <graphics/Renderer.h>
#include <Application.h>
#include <display/Window.h>
#include <iostream>

//class Application; // Forward this to not loop any weird header inclusions

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

std::vector<Drawable> persist_drawables = std::vector<Drawable>();

void Renderer::startRender() {
	if (!targetWin->focused() && !targetWin->renderOutOfFocus) {
		pendingRequests.clear(); //? Maybe not a smart idea, gotta make sure we have sort of 2 render queues, especially for the double buffering shittamajig
		return;
	}
	if (SDL_GL_MakeCurrent(targetWin->handle, targetWin->context) != 0)
		std::cout << "whoopsie daisy, couldn't make current opengl context to window: " << targetWin->name().c_str() << "\n";

	glClearColor(0.7f, 0.2f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	persist_drawables.clear();
	for (Drawable d : drawables) {
		d.prepareDraw();
		if (!d.usesEBO) {
			CHECK_GL(glDrawArrays(d.vert_type, 0, d.verts.size()))
		}
		else {
			CHECK_GL(glDrawElements(d.vert_type, d.vert_indices.size(), GL_UNSIGNED_INT, NULL));
		}
		d.finishDraw();
		if (d.persist) persist_drawables.push_back(d); // ?
	}
	drawables = persist_drawables; // ?
}

void Renderer::endRender() {
	glBindVertexArray(0);
	SDL_GL_SwapWindow(targetWin->handle);
}