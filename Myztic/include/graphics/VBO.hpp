#pragma once

#include <ErrorHandler.hpp>
#include <array>

class VBO {
public:
	inline VBO(GLuint vbo) { this->vbo = vbo; };
	inline VBO() = default;

	inline static VBO make() {
		VBO ret;
		CHECK_GL(glGenBuffers(1, &ret.vbo));
		return ret;
	};

	//this must be written here due to linker complications
	template <int AMOUNT>
	inline static std::array<VBO, AMOUNT> makeNum() {
		std::array<VBO, AMOUNT> rets;

		for (int i = 0; i < AMOUNT; i++) 
			CHECK_GL(glGenBuffers(1, &rets[i].vbo));

		return rets;
	};

	inline static void unbind() {
		CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	};

	inline void bind() {
		CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	};

	inline void destroy() {
		int buf;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buf);

		if (buf == vbo) {
			printf("[[WARNING]]: TRYING TO DESTROY THE CURRENTLY BOUND BUFFER: %i, UNBINDING BUFFER\n", buf);
			VBO::unbind();
		}

		CHECK_GL(glDeleteBuffers(1, &vbo));
	};

	inline void fill(GLfloat* vertices, int arrayLength, GLenum drawType) {
		int buf;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buf);

		if (buf != vbo)
			printf("[[WARNING]]: TRYING TO MODIFY CURRENTLY BOUND VERTEX BUFFER: %i FROM A NOT BOUND BUFFER CLASS: %u\n", buf, vbo);

		CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * arrayLength, (void*)vertices, drawType));
	};
private:
	GLuint vbo;
};