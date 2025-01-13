#include "pch.h"
#include <graphics\Drawable.h>

Drawable::Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties) {
	this->drawTarget = drawerWin;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	verts = std::vector<GLfloat>();
	this->vbo = VBO::make();
}

Drawable::Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties, std::vector<GLfloat>& vertData) {
	this->drawTarget = drawerWin;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	verts = vertData;
	this->vbo = VBO::make();
}

Drawable::~Drawable() {
	drawTarget = nullptr;
}

Drawable Drawable::makeTriStrip(Window* drawerWin, std::vector<GLfloat>& verts) {
	std::vector<InputProperty> vec = { ShaderInputLayout::POSITION };

	Drawable obj = Drawable(drawerWin, vec, verts);
	obj.vert_type = GL_TRIANGLE_STRIP;
	obj.vbo.bind();
	obj.vbo.fill(verts.data(), sizeof(GLfloat) * verts.size(), GL_STATIC_DRAW);

	obj.inputLayout.setVertexLayout();
	obj.inputLayout.enableAllAttribs();

	Shader vs = Shader(GL_VERTEX_SHADER, "assets/shaders/vs.glsl");
	Shader fs = Shader(GL_FRAGMENT_SHADER, "assets/shaders/fs.glsl");
	obj.shaderProgram.attach(vs);
	obj.shaderProgram.attach(fs);
	obj.shaderProgram.link();
	vs.deleteShader();
	fs.deleteShader();

	obj.inputLayout.unbind(); // By unbinding the layout / vao before anything else we dont need to always manually bind the vbo (or other buffers) on draw
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