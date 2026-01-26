#include "pch.h"

#include <graphics/Drawable.h>
#include <graphics/primitives/Line.h>
#include <graphics/PrecompiledShaders.h>

using namespace Myztic;

Line* Line::createLine(Scene* linkedScene, glm::vec2 startpoint, glm::vec2 endpoint, glm::vec4 color) {
	glm::vec3 sp = glm::vec3(startpoint, 0.f);
	glm::vec3 ep = glm::vec3(endpoint, 0.f);
	VertexBuffer vbuf(std::move(VertexLayout{}.Append(VertexLayout::Position3D).Append(VertexLayout::Float4Color)));
	vbuf.EmplaceBack(sp, color);
	vbuf.EmplaceBack(ep, color);

	Line* ret = new Line(linkedScene, vbuf, sp, ep, color);

	return ret;
}

Line* Myztic::Line::createRay(Scene* linkedScene, glm::vec3 startpoint, glm::vec3 endpoint, glm::vec4 color) {
	VertexBuffer vbuf(std::move(VertexLayout{}.Append(VertexLayout::Position3D).Append(VertexLayout::Float4Color)));
	vbuf.EmplaceBack(startpoint, color);
	vbuf.EmplaceBack(endpoint, color);

	Line* ret = new Line(linkedScene, vbuf, startpoint, endpoint, color);

	return ret;
}

glm::vec3 Line::get_startpoint()
{
	return this->u_start;
}

glm::vec3 Line::get_endpoint()
{
	return this->u_end;
}

glm::vec4 Line::get_color()
{
	return this->u_color;
}

void Line::set_startpoint(glm::vec3 new_startpoint)
{
	vbo.bind();
	vbo.update(&new_startpoint, 0, sizeof(glm::vec3));
	vbo.unbind();
	this->u_start = new_startpoint;
}

void Line::set_endpoint(glm::vec3 new_endpoint)
{
	vbo.bind();
	vbo.update(&new_endpoint, sizeof(glm::vec3)+sizeof(glm::vec4), sizeof(glm::vec3));
	vbo.unbind();
	this->u_end = new_endpoint;
}

void Line::set_color(glm::vec4 new_color) {
	vbo.bind();
	//i should probably make updating vertex buffers easier using shaderinputlayout or something?
	vbo.update(&new_color, sizeof(glm::vec3), sizeof(glm::vec4));
	vbo.update(&new_color, sizeof(glm::vec3)*2+sizeof(glm::vec4), sizeof(glm::vec4));
	vbo.unbind();
	this->u_color = new_color;
}

Line::Line(Scene* linkedScene, VertexBuffer& vbuf, glm::vec3 startpoint, glm::vec3 endpoint, glm::vec4 color, std::vector<Shader> shaders) : Drawable(linkedScene, vbuf), u_start(startpoint), u_end(endpoint), u_color(color) {
	vert_type = GL_LINES;
	vbo.bind();

	//Dynamic draw so you can freely change the vertices of the line (?)
	vbo.fill((void*)vbuf.GetData(), vbuf.SizeBytes(), GL_DYNAMIC_DRAW);

	inputLayout.setVertexLayout();
	inputLayout.enableAllAttribs();

	for (Shader shader : shaders)
		shaderProgram.attach(shader);
	
	shaderProgram.link();

	inputLayout.unbind();
	vbo.unbind();
	shaderProgram.unbind();
};