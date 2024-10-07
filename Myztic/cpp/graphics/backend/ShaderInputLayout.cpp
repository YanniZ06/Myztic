#include "pch.h"
#include <Windows.h>
#include <graphics/backend/ShaderInputLayout.h>

// -9 pointer offset?? really????? | nvm it changed nothing

InputProperty ShaderInputLayout::POSITION = { Position, 3, GL_FLOAT, 0, sizeof(GLfloat) };
InputProperty ShaderInputLayout::COLOR = { Color, 3, GL_FLOAT, 0, sizeof(GLfloat) };
InputProperty ShaderInputLayout::TEXCOORD = { TextureCoordinate, 3, GL_FLOAT, 0, sizeof(GLfloat) };
InputProperty ShaderInputLayout::NORMAL = { Normal, 3, GL_FLOAT, 0, sizeof(GLfloat) };


LayoutDescription ShaderInputLayout::createLayoutDescription(std::vector<InputProperty>& inputProperties) {
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

ShaderInputLayout::ShaderInputLayout() {
	this->propertyCount = 0;
	this->attachedVAO = VAO::make();
};

void ShaderInputLayout::deleteInputLayout() {
	attachedVAO.destroy();
	ZeroMemory(&description, sizeof(LayoutDescription));
	propertyCount = 0;
};

void ShaderInputLayout::enableAttrib(int index) {
	if (index > propertyCount) {
		throw "Could not enable attrib index greater than the max attrib count";
	};

	CHECK_GL(glEnableVertexAttribArray(index));
};

void ShaderInputLayout::enableAllAttribs() {
	for (int i = 0; i < propertyCount; i++) {
		CHECK_GL(glEnableVertexAttribArray(i));
	}
};

void ShaderInputLayout::disableAttrib(int index) {
	if (index > propertyCount) {
		throw "Could not disable attrib index greater than the max attrib count";
	};

	CHECK_GL(glDisableVertexAttribArray(index));
};

void ShaderInputLayout::disableAllAttribs() {
	for (int i = 0; i < propertyCount; i++) {
		CHECK_GL(glDisableVertexAttribArray(i));
	}
};

ShaderInputLayout ShaderInputLayout::createInputLayout(LayoutDescription description) {
	GLint previousVAO;
	CHECK_GL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previousVAO));

	ShaderInputLayout ret = ShaderInputLayout();
	ret.bindInputLayout();

	for (int i = 0; i < description.inputProperties.size(); i++) {
		InputProperty prop = description.inputProperties[i];
		
		CHECK_GL(glVertexAttribPointer(ret.propertyCount, prop.size, prop.glType, GL_FALSE,
			description.elementCount * prop.sizeOfType, (void*)(prop.pointerOffset * prop.sizeOfType)));
			ret.propertyCount++;
	};

	ret.description = description;

	if (previousVAO != 0) {
		CHECK_GL(glBindVertexArray(previousVAO));
	};

	return ret;
};