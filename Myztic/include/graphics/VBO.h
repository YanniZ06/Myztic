#pragma once
#include "glad.h"

class VBO {
public:
	VBO(GLuint vbo);
	~VBO();
	static VBO make();
	static VBO* makeNum(unsigned int amount);
	static void unbind();
	void bind();
	void del();
	void fill(GLfloat* vertices, GLint drawType);
private:
	GLuint vbo;
};