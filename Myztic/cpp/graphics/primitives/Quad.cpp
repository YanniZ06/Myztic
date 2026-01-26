#include <pch.h>

#include <graphics\Drawable.h>
#include <display\Window.h>
#include <graphics\primitives\Quad.h>

using namespace Myztic;

inline VertexBuffer createVertexBuffer(float x, float y, float z, float width, float height, glm::vec4 color) {
	//prepare the list
	VertexBuffer data = VertexBuffer(std::move(VertexLayout{}.Append(VertexLayout::Position3D).Append(VertexLayout::Float4Color)));
	//write vertices
	//top left
	data.EmplaceBack(glm::vec3(x, y, z), color);
	//bottom left
	data.EmplaceBack(glm::vec3(x, y - height, z), color);
	//bottom right
	data.EmplaceBack(glm::vec3(x + width, y - height, z), color);
	//top right
	data.EmplaceBack(glm::vec3(x + width, y, z), color);

	return data;
}

inline void updateBuffer(VBO& vbo, VertexBuffer& vbuf, float x, float y, float z, float width, float height, glm::vec4 color) {
	vbuf = createVertexBuffer(x, y, z, width, height, color);
	vbo.bind();
	vbo.update((void*)vbuf.GetData(), 0, vbuf.SizeBytes());
	vbo.unbind();
}

//if you wish to create a rectangle (IN AN ORTHOGRAPHIC PLANE, NOT A PERSPECTIVE ONE), you would invert the height (-height) so that the rectangle can have proper coordinates and its top left vertex be the anchor point
Quad* Quad::makeQuad(Scene* linkedScene, float x, float y, float width, float height, float z, glm::vec4 color) {
	//need to feed in EBO data
	VertexBuffer vbuf = createVertexBuffer(x, y, z, width, height, color);
	Quad* ret = new Quad(linkedScene, vbuf, x, y, width, height, z, {PrecompiledShaders::color_vs, PrecompiledShaders::color_fs}, color);
	ret->useEBO({
		//front face (counter-clockwise)
		0, 1, 2, 0, 2, 3
		//back face? (clockwise ordering)
		//,0, 3, 2, 0, 2, 1
		});

	return ret;
}

float Quad::get_x() { return x; }
float Quad::get_y() { return y; }
float Quad::get_z() { return z; }
float Quad::get_width() { return width; }
float Quad::get_height() { return height; }
glm::vec4 Quad::get_color() { return color; }

void Quad::set_x(float new_x) { 
	updateBuffer(vbo, vertexData, new_x, y, z, width, height, color);
	this->x = new_x;
}

void Quad::set_y(float new_y) { 
	updateBuffer(vbo, vertexData, x, new_y, z, width, height, color);
	this->y = new_y;
}

void Quad::set_z(float new_z) { 
	updateBuffer(vbo, vertexData, x, y, new_z, width, height, color);
	this->z = new_z;
}

void Quad::set_width(float new_width) { 
	updateBuffer(vbo, vertexData, x, y, z, new_width, height, color);
	this->width = new_width;
}

void Quad::set_height(float new_height) { 
	updateBuffer(vbo, vertexData, x, y, z, width, new_height, color);
	this->height = new_height;
}

void Quad::set_color(glm::vec4 new_color) {
	updateBuffer(vbo, vertexData, x, y, z, width, height, new_color);
	this->color = new_color;
}

Quad::Quad(Scene* linkedScene, VertexBuffer& vbuf, float x, float y, float width, float height, float z, std::vector<Shader> shaders, glm::vec4 color) : Drawable(linkedScene, vbuf), x(x), y(y), z(z), width(width), height(height), color(color) {
	this->vert_type = GL_TRIANGLES;
	vbo.bind();

	//GL_DYNAMIC_DRAW just incase you want to manipulate width and height (im pretty sure you can work coordinates by translation regardless?)
	vbo.fill((void*)vbuf.GetData(), vbuf.SizeBytes(), GL_DYNAMIC_DRAW);

	inputLayout.setVertexLayout();
	inputLayout.enableAllAttribs();

	//i hate doing this fucking loop
	for (Shader shader : shaders)
		shaderProgram.attach(shader);

	shaderProgram.link();

	inputLayout.unbind();
	vbo.unbind();
	shaderProgram.unbind();
}