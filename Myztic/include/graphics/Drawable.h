#pragma once

#include <graphics/backend/VBO.hpp>
//#include <display/Window.h>
#include <graphics/backend/Shader.h>
#include <graphics/backend/ShaderInputLayout.h>
#include <graphics/backend/ShaderProgram.h>
#include <graphics/backend/EBO.hpp>
#include <glm.hpp>
#include <glad.h>
#include <graphics\Vertex.h>
//#include <graphics/Renderer.h>

class Scene;
class Camera;
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
	Drawable(Scene* linkedScene, std::vector<InputProperty>& inputProperties);
	Drawable(Scene* linkedScene, VertexBuffer& vertData);
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
	/**  
	* Instantiates a quad Drawable.
	* \param linkedScene The scene this Drawable is linked to/Drawn in.
	* \param verts The VertexBuffer object describing the input layout; for example: a written template: `VertexBuffer(VertexLayout{}.Append(VertexLayout::Position3D));`
	* \param shaders A vector of Shader objects to be linked with the ShaderProgram, will NOT be deleted after attachment, if not used from PrecompiledShaders, you may delete them, otherwise, NEVER DELETE PRECOMPILED SHADERS.
	*/
	static Drawable makeQuad(Scene* linkedScene, VertexBuffer& verts, std::vector<Shader> shaders);

	// ???
	/// Should persist in the queued drawables list in the renderer after the frame has been drawn or should be taken off the list.
	bool persist = true;
	//The camera the object is bound to.
	Camera* camera = nullptr;
	//The transformation matrix for the object.
	glm::mat4 transformation = glm::mat4(1.f);
protected:
	/// The window that is being drawn to
	Scene* linkedScene;
	
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
