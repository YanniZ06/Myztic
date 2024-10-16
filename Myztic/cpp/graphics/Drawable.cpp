#include "pch.h"
#include <graphics\Drawable.h>

Drawable::Drawable(Window* targetWin, std::vector<InputProperty>& inputProperties) {
	this->drawTarget = targetWin;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	this->vbo = VBO::make();
	// this->vao = VAO::make();
}

Drawable::~Drawable() {
	drawTarget = nullptr;
}

// prepare draw
void Drawable::prepareDraw() {
	shaderProgram.bind();
	inputLayout.bindInputLayout();
	// vao.bind();
}

void Drawable::finishDraw() {
	shaderProgram.unbind();
	inputLayout.unbind();
}
