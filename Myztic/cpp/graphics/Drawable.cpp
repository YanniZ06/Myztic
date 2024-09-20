#include "pch.h"
#include <graphics\Drawable.h>

Drawable::Drawable(Window* targetWin, std::vector<InputProperty>& inputProperties) {
	this->drawTarget = targetWin;
	this->inputLayout = ShaderInputLayout::createInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	this->vbo = VBO::make();
}

void Drawable::draw() {
	shaderProgram.bind();
	inputLayout.bindInputLayout();
}