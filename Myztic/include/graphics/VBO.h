#pragma once

#include <glad.h>
#include <array>
#include <exception>
#include <ErrorHandler.hpp>

class VBO {
public:
	VBO(GLuint vbo);
	VBO() = default;
	static VBO make();

	//this must be written here due to linker complications
	template <int AMOUNT>
	static std::array<VBO, AMOUNT> makeNum() {
		std::array<VBO, AMOUNT> rets;

		for (int i = 0; i < AMOUNT; i++) {
			unsigned int vbo;
			CHECK_GL(glGenBuffers(1, &vbo));
			rets[i] = VBO(vbo);
		}

		return rets;
	};

	static void unbind();
	void bind();
	void destroy();
	void fill(GLfloat* vertices, int arrayLength, GLenum drawType);
private:
	GLuint vbo;
};