#include "pch.h"
#include <graphics\Drawable.h>

Drawable::Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties) {
	this->drawTarget = drawerWin;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	verts = std::vector<Vertex>();
	this->vbo = VBO::make();
}

Drawable::Drawable(Window* drawerWin, std::vector<InputProperty>& inputProperties, std::vector<Vertex>& vertData) {
	this->drawTarget = drawerWin;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	//copy operation, safe to get rid of the original variable.
	verts = vertData;
	this->vbo = VBO::make();
}

Drawable::~Drawable() {
	drawTarget = nullptr;
}

Drawable Drawable::makeQuad(Window* drawer_win, std::vector<InputProperty>& inputProperties, std::vector<Vertex>& verts) {
	Drawable obj = Drawable(drawer_win, inputProperties, verts);
	obj.vert_type = GL_TRIANGLES;
	obj.vbo.bind();
	unsigned long long actualSize = 0;
	for (int i = 0; i < inputProperties.size(); i++) {
		actualSize += (inputProperties[i].sizeOfType * inputProperties[i].size);
	};
	//reconsider this especially for GL_STATIC_DRAW.
	obj.vbo.fill(verts.data(), actualSize * verts.size(), GL_STATIC_DRAW);

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