#pragma once

#include <graphics/backend/VBO.hpp>
//#include <display/Window.h>
#include <graphics/backend/Shader.h>
#include <graphics/backend/ShaderInputLayout.h>
//#include <graphics/Renderer.h>

class Window;
// todo: figure out which of these need to be protected in particular
class Drawable {
	friend class Application;
public:
	Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties);
	Drawable() = default;

	/// The vertex buffer object used for OpenGL draw calls.
	VBO vbo;
	/// The shader input layout used to define shader attributes for OpenGL.
	ShaderInputLayout inputLayout;
	/// The shader program used to handle shaders for OpenGL draw calls.
	ShaderProgram shaderProgram;

	/// Should persist in the queued drawables list in the renderer after the frame has been drawn or should be taken off the list.
	bool persist = true;
protected:
	/// The window that is being drawn to
	Window* drawTarget;
	
	/**
	 * Renders this drawable. Debatable if useful???
	 * 
	 */
	 void draw();
};
