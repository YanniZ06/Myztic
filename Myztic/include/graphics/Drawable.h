#pragma once

#include <graphics/backend/VBO.hpp>
#include <display/Window.h>
#include <graphics/backend/Shader.h>
#include <graphics/backend/ShaderInputLayout.h>

// todo: figure out which of these need to be protected in particular
class Drawable {
public:
	Drawable(Window* drawerWin);
	Drawable() = default;

	/// The vertex buffer object used for OpenGL draw calls.
	VBO vbo;
	/// The shader input layout used to define shader attributes for OpenGL.
	ShaderInputLayout inputLayout;
	/// The shader program used to handle shaders for OpenGL draw calls.
	ShaderProgram shaderProgram;
protected:
	/// The window that is being drawn to
	Window* drawTarget;
	/**
	 * Renders this drawable. Debateable if useful???
	 * 
	 */
	// void draw();
};
