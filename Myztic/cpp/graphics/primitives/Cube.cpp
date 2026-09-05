#include "pch.h"
#include <graphics\Drawable.h>
#include <graphics\TexturedDrawable.h>
#include <display\Window.h>
#include <graphics\primitives\Cube.h>
#include <gtc/matrix_transform.hpp>

using namespace Myztic;

Cube* Cube::makeCube(Scene* linkedScene, float x, float y, float z, float size, glm::vec4 color, std::vector<Shader> shaders)
{
	VertexBuffer vbuf = VertexBuffer(VertexLayout{}.Append(VertexLayout::Position3D).Append(VertexLayout::Float4Color));
	vbuf.EmplaceBack(glm::vec3(x, y, z), color);
	vbuf.EmplaceBack(glm::vec3(x, y + 1, z), color);
	vbuf.EmplaceBack(glm::vec3(x + 1, y + 1, z), color);
	vbuf.EmplaceBack(glm::vec3(x + 1, y, z), color);
	vbuf.EmplaceBack(glm::vec3(x, y, z - 1), color);
	vbuf.EmplaceBack(glm::vec3(x, y + 1, z - 1), color);
	vbuf.EmplaceBack(glm::vec3(x + 1, y + 1, z - 1), color);
	vbuf.EmplaceBack(glm::vec3(x+1, y, z-1), color);
	Cube* ret = new Cube(linkedScene, vbuf, size, color, shaders);
	ret->vertexData = vbuf;
	ret->useEBO({
		0, 1, 2, 0, 2, 3, 
		4, 5, 6, 4, 6, 7, 
		4, 0, 3, 4, 3, 7, 
		4, 5, 1, 4, 1, 0, 
		3, 2, 6, 3, 6, 7, 
		5, 1, 2, 5, 2, 6
	});
	
	ret->transformation = glm::scale(ret->transformation, glm::vec3(size, size, size));

	return ret;
}

Cube* Myztic::Cube::makeCube(Scene* linkedScene, glm::vec3 diagonalPoint1, glm::vec3 diagonalPoint2, float size, glm::vec4 color, std::vector<Shader> shaders)
{
	VertexBuffer vbuf = VertexBuffer(VertexLayout{}.Append(VertexLayout::Position3D).Append(VertexLayout::Float4Color));
	vbuf.EmplaceBack(diagonalPoint1, color); //0
	vbuf.EmplaceBack(glm::vec3(diagonalPoint1.x, diagonalPoint1.y, diagonalPoint2.z), color); //1
	vbuf.EmplaceBack(glm::vec3(diagonalPoint1.x, diagonalPoint2.y, diagonalPoint1.z), color); //2
	vbuf.EmplaceBack(glm::vec3(diagonalPoint1.x, diagonalPoint2.y, diagonalPoint2.z), color); //3
	vbuf.EmplaceBack(glm::vec3(diagonalPoint2.x, diagonalPoint1.y, diagonalPoint1.z), color); //4
	vbuf.EmplaceBack(glm::vec3(diagonalPoint2.x, diagonalPoint1.y, diagonalPoint2.z), color); //5
	vbuf.EmplaceBack(glm::vec3(diagonalPoint2.x, diagonalPoint2.y, diagonalPoint1.z), color); //6
	vbuf.EmplaceBack(diagonalPoint2, color); //7
	Cube* ret = new Cube(linkedScene, vbuf, size, color, shaders);
	ret->vertexData = vbuf;
	ret->useEBO({
		0, 1, 3, 0, 3, 2,
		4, 6, 7, 4, 7, 5,
		0, 4, 5, 0, 5, 1,
		2, 3, 7, 2, 7, 6, 
		0, 2, 6, 0, 6, 4, 
		1, 5, 7, 1, 7, 3
	});

	ret->set_size(glm::vec3(size, size, size));

	return ret;
}

Cube::Cube(Scene* linkedScene, VertexBuffer& vbuf, float size, glm::vec4 color, std::vector<Shader> shaders) : TexturedDrawable(linkedScene, vbuf), size(size), color(color)
{
	this->vert_type = GL_TRIANGLES;
	vbo.bind();

	//GL_STATIC_DRAW for the most part?
	//need to make a system that maps back changes in the vbuf to the vbo if needed directly? (make something for Position3D for example where you can easily change x, y, and z and it instantly maps back changes into the vbo
	vbo.fill((void*)vbuf.GetData(), vbuf.SizeBytes(), GL_STATIC_DRAW);

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

glm::vec4 Cube::get_color()
{
	return this->color;
}

void Myztic::Cube::set_color(glm::vec4 new_col)
{
	this->color = new_col;
}
