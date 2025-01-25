#pragma once

#include <graphics/backend/VBO.hpp>
//#include <display/Window.h>
#include <graphics/backend/Shader.h>
#include <graphics/backend/ShaderInputLayout.h>
#include <graphics/backend/ShaderProgram.h>
#include <graphics/backend/EBO.hpp>

#include <glad.h>
#include <graphics\Vertex.h>
//#include <graphics/Renderer.h>

class Window;

//? WIP
/*
enum PixelsType {
	P_STATIC, P_DYNAMIC
};
*/

// todo: figure out which of these need to be protected in particular
class Drawable {
	friend class Application;
	friend class Renderer;
public:
	Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties);
	Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties, VertexBuffer& vertData);
	~Drawable();

	//? raw pixel data, this isnt thought out much yet, will do with ziad
	//? hello this is ziad, this should ONLYY be available when GL_DYNAMIC_DRAW is input.
	void* pixelData = nullptr;
	//! PixelsType pixelInfo;

	/// Vertices for our drawable
	VertexBuffer vertexData = VertexBuffer(VertexLayout{}.Append(VertexLayout::Position3D));
	/// The type of primitive a set of vertices represents -- must be a value accepted by glDrawElements as its first parameter
	int vert_type = GL_TRIANGLES;
	/// Vertex-specific indices for EBO usage -- requires a useEBO() call to initialize.
	std::vector<GLuint> vert_indices;
	bool usesEBO = false;

	/// The vertex buffer object used for OpenGL draw calls.
	VBO vbo;
	/// The element buffer object, mainly used for indices -- requires a useEBO() call to initialize.
	EBO ebo;
	/// The shader input layout used to define shader attributes for OpenGL -- Also handles the vertex array object (VAO).
	ShaderInputLayout inputLayout;
	/// The shader program used to handle shaders for OpenGL draw calls.
	ShaderProgram shaderProgram;

	/// Inlined, toggles EBO usage [currently only toggles on]
	void useEBO();
	static Drawable makeQuad(Window* drawerWin, VertexBuffer& verts);

	// ???
	/// Should persist in the queued drawables list in the renderer after the frame has been drawn or should be taken off the list.
	bool persist = true;
protected:
	/// The window that is being drawn to
	Window* drawTarget;
	
	/// Inlined, retrieves the VAO from the inputLayout
	// VAO* getVAO();
	// THIS IS A TEMPORARY SOLUTION, WILL ADAPT OUR DRAWING PRINCIPLE SOON 
	void prepareDraw();
	void finishDraw();
};

// todo: logic to untoggle 
inline void Drawable::useEBO() {
	usesEBO = true;
	vert_indices = std::vector<GLuint>();
	this->ebo = EBO::make();
}
