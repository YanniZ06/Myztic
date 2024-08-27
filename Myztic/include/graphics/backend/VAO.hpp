#pragma once

#include <ErrorHandler.hpp>
#include <array>

class VAO {
public:
	inline VAO(GLuint handle) { this->vao = handle; }
	inline VAO() = default;

	inline static VAO make() {
		//this uses the default constructor, that's why its required
		VAO ret;
		CHECK_GL(glGenVertexArrays(1, &ret.vao));
		return ret;
	};

	template <int AMOUNT>
	inline static std::array<VAO, AMOUNT> makeNum() {
		//this uses the default constructor too !!!
		std::array<VAO, AMOUNT> rets;

		for (int i = 0; i < AMOUNT; i++)
			CHECK_GL(glGenVertexArrays(1, &rets[i].vao));
		
		return rets;
	};

	inline static void unbind() {
		CHECK_GL(glBindVertexArray(0));
	};

	inline void bind() {
		CHECK_GL(glBindVertexArray(vao));
	};

	inline void destroy() {
		int buf;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &buf);

		if (buf == vao) {
			printf("[[WARNING]]: TRYING TO DESTROY THE CURRENTLY BOUND VERTEX ARRAY OBJECT: %i, UNBINDING OBJECT\n", buf);
			VAO::unbind();
		}

		CHECK_GL(glDeleteVertexArrays(1, &vao));
	};

private:
	GLuint vao;
};