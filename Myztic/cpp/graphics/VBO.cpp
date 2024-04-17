#include "pch.h"
#include <graphics/VBO.h>

VBO::VBO(GLuint vbo) : vbo(vbo) {}

VBO VBO::make() {
	VBO ret = VBO(-99);
	CHECK_GL(glGenBuffers(1, &ret.vbo));
	return ret;
}

void VBO::unbind() {
	CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VBO::bind() {
	CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
}

void VBO::destroy() {
	int buf;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buf);

	if (buf == vbo) {
		printf("[[WARNING]]: TRYING TO DESTROY THE CURRENTLY BOUND BUFFER: %i, UNBINDING BUFFER", buf);
		VBO::unbind();
	}

	CHECK_GL(glDeleteBuffers(1, &vbo));
}

void VBO::fill(GLfloat* vertices, int arrayLength, GLenum drawType) {
	int buf;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buf);

	if (buf != vbo) 
		printf("[[WARNING]]: TRYING TO MODIFY CURRENTLY BOUND VERTEX BUFFER: %i FROM A NOT BOUND BUFFER CLASS: %u", buf, vbo);
	
	CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * arrayLength, (void*)vertices, drawType));
}