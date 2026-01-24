#include "pch.h"

#include <graphics/primitives/Line.h>
#include <graphics/PrecompiledShaders.h>

using namespace Myztic;

Line* Line::createLine(Scene* linkedScene, glm::vec2 startpoint, glm::vec2 endpoint) {
	VertexBuffer vbuf(std::move(VertexLayout{}.Append(VertexLayout::Position2D)));
	vbuf.EmplaceBack(startpoint);
	vbuf.EmplaceBack(endpoint);

	Line* ret = new Line(linkedScene, vbuf);
	ret->u_start = startpoint;
	ret->u_end = endpoint;

	return ret;
}

glm::vec2 Myztic::Line::get_startpoint()
{
	return this->u_start;
}

glm::vec2 Myztic::Line::get_endpoint()
{
	return this->u_end;
}

void Myztic::Line::set_startpoint(glm::vec2 new_startpoint)
{
	vbo.bind();
	vbo.update(&new_startpoint, 0, sizeof(glm::vec2));
	vbo.unbind();
	this->u_start = new_startpoint;
}

void Myztic::Line::set_endpoint(glm::vec2 new_endpoint)
{
	vbo.bind();
	vbo.update(&new_endpoint, sizeof(glm::vec2), sizeof(glm::vec2));
	vbo.unbind();
	this->u_end = new_endpoint;
}

Line::Line(Scene* linkedScene, VertexBuffer& vbuf) : Drawable(linkedScene, vbuf) {
	vert_type = GL_LINES;
	vbo.bind();

	//Dynamic draw so you can freely change the vertices of the line (?)
	vbo.fill((void*)vbuf.GetData(), vbuf.SizeBytes(), GL_DYNAMIC_DRAW);

	inputLayout.setVertexLayout();
	inputLayout.enableAllAttribs();

	shaderProgram.attach(PrecompiledShaders::line_vs);
	shaderProgram.attach(PrecompiledShaders::line_fs);

	shaderProgram.link();

	inputLayout.unbind();
	vbo.unbind();
	shaderProgram.unbind();
};