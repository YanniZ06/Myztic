#include "pch.h"
#include <Windows.h>
#include <graphics/backend/ShaderInputLayout.h>

// -9 pointer offset?? really????? | nvm it changed nothing

Myztic::InputProperty Myztic::ShaderInputLayout::POSITION = { Position, 3, GL_FLOAT, 0, sizeof(GLfloat) };
Myztic::InputProperty Myztic::ShaderInputLayout::COLOR = { Color, 4, GL_FLOAT, 0, sizeof(GLfloat) };
Myztic::InputProperty Myztic::ShaderInputLayout::TEXCOORD = { TextureCoordinate, 2, GL_FLOAT, 0, sizeof(GLfloat) };
Myztic::InputProperty Myztic::ShaderInputLayout::NORMAL = { Normal, 3, GL_FLOAT, 0, sizeof(GLfloat) };

Myztic::ShaderInputLayout::ShaderInputLayout(Myztic::LayoutDescription description) {
	GLint previousVAO;
	CHECK_GL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previousVAO));

	this->propertyCount = 0;
	this->attachedVAO = VAO::make();
	this->bindInputLayout();

	this->description = description;

	if (previousVAO != 0) {
		CHECK_GL(glBindVertexArray(previousVAO));
	};
};

Myztic::LayoutDescription Myztic::ShaderInputLayout::createLayoutDescription(std::vector<Myztic::InputProperty>& inputProperties) {
	GLint elementCount = 0;

	for (int i = 0; i < inputProperties.size(); i++) {
		inputProperties[i].pointerOffset = elementCount;
		elementCount += inputProperties[i].size;
	}

	LayoutDescription desc;
	desc.elementCount = elementCount;
	desc.inputProperties = inputProperties;

	return desc;
};

void Myztic::ShaderInputLayout::setVertexLayout()
{
	for (int i = 0; i < description.inputProperties.size(); i++) {
		InputProperty prop = description.inputProperties[i];

		CHECK_GL(glVertexAttribPointer(this->propertyCount, prop.size, prop.glType, GL_FALSE,
			description.elementCount * prop.sizeOfType, (void*)(prop.pointerOffset * prop.sizeOfType)));
		this->propertyCount++;
	};
};

void Myztic::ShaderInputLayout::deleteInputLayout() {
	attachedVAO.destroy();
	ZeroMemory(&description, sizeof(LayoutDescription));
	propertyCount = 0;
};

void Myztic::ShaderInputLayout::enableAttrib(int index) {
	if (index > propertyCount) {
		throw "Could not enable attrib index greater than the max attrib count";
	};

	CHECK_GL(glEnableVertexAttribArray(index));
};

void Myztic::ShaderInputLayout::enableAllAttribs() {
	for (int i = 0; i < propertyCount; i++) {
		CHECK_GL(glEnableVertexAttribArray(i));
	}
};

void Myztic::ShaderInputLayout::disableAttrib(int index) {
	if (index > propertyCount) {
		throw "Could not disable attrib index greater than the max attrib count";
	};

	CHECK_GL(glDisableVertexAttribArray(index));
};

void Myztic::ShaderInputLayout::disableAllAttribs() {
	for (int i = 0; i < propertyCount; i++) {
		CHECK_GL(glDisableVertexAttribArray(i));
	}
};
