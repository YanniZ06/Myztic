#include "pch.h" 

#include <graphics/Renderer.h>
#include <Application.h>

//class Application; // Forward this to not loop any weird header inclusions

std::map<Window*, std::vector<RenderRequestBase>> Renderer::pendingRequests;

void Renderer::drawLine(glm::vec2 p1, glm::vec2 p2, Window* drawTarget)
{
	DLineRequest<glm::vec2> req(p1, p2);
	// print : req.type();
	pendingRequests[drawTarget].push_back(req.base());
}

void Renderer::drawRay(glm::vec3 p1, glm::vec3 p2, Window* drawTarget)
{
	DLineRequest<glm::vec3> req(p1, p2);
	// print : req.type();
	pendingRequests[drawTarget].push_back(req.base());
}

// This is the full renderer logic that is used in Application's render phase

void Renderer::startRender() {
	// Render ALL windows after one another
	for (std::map<unsigned char, std::shared_ptr<Window>>::const_iterator it = Application::windows.begin(); it != Application::windows.end(); ++it)
	{
		Window* win = it->second.get();

		if (!win->focused() && !win->renderOutOfFocus) {
			pendingRequests[win].clear(); //? Maybe not a smart idea, gotta make sure we have sort of 2 render queues, especially for the double buffering shittamajig
			continue;
		}
	}
}

