#include "pch.h"
#include <graphics\Drawable.h>

Drawable::Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties) {
	this->drawTarget = drawerWin;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	vertexData = VertexBuffer(std::move(VertexLayout{}.Append(VertexLayout::Position3D)));
	this->vbo = VBO::make();
}

Drawable::Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties, VertexBuffer& vertData) {
	this->drawTarget = drawerWin;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	//copy operation, safe to get rid of the original variable.
	vertexData = vertData;
	this->vbo = VBO::make();
}

Drawable::~Drawable() {
	drawTarget = nullptr;
}

Drawable Drawable::makeQuad(Window* drawer_win, VertexBuffer& verts) {
	std::vector<InputProperty> ips = verts.GetLayout().GetDescription().inputProperties;
	Drawable obj = Drawable(drawer_win, ips, verts);
	obj.vert_type = GL_TRIANGLES;
	obj.vbo.bind();
	//reconsider this especially for GL_STATIC_DRAW.
	obj.vbo.fill((void*)verts.GetData(), verts.SizeBytes(), GL_STATIC_DRAW);

	obj.inputLayout.setVertexLayout();
	obj.inputLayout.enableAllAttribs();

	obj.useEBO();

	obj.vert_indices = std::vector<GLuint>({ 0, 1, 3, 1, 2, 3 });

	obj.ebo.bind();
	obj.ebo.fill(obj.vert_indices.data(), sizeof(GLuint) * obj.vert_indices.size(), GL_STATIC_DRAW);

	Shader vs = Shader(GL_VERTEX_SHADER, "assets/shaders/vs.glsl");
	Shader fs = Shader(GL_FRAGMENT_SHADER, "assets/shaders/fs.glsl");
	obj.shaderProgram.attach(vs);
	obj.shaderProgram.attach(fs);
	obj.shaderProgram.link();
	vs.deleteShader();
	fs.deleteShader();

	obj.inputLayout.unbind();
	obj.vbo.unbind();

	return obj;
}

// prepare draw
void Drawable::prepareDraw() {
	shaderProgram.bind();
	inputLayout.bindInputLayout();
}

void Drawable::finishDraw() {
	shaderProgram.unbind();
	inputLayout.unbind();
}