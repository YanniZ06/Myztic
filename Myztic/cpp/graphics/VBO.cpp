#include "pch.h"
#include "graphics/VBO.h"
#include "ErrorHandler.hpp"

VBO::VBO(GLuint vbo) : vbo(vbo) {}

VBO::~VBO()
{
	//delete
}

VBO VBO::make() {
	VBO ret = VBO(-99);
	glGenBuffers(1, &ret.vbo);
	CHECK_GL();
	if (!glIsBuffer(ret.vbo)) throw "Produced VBO is not a buffer";
	return ret;
}