#pragma once

#include <ErrorHandler.hpp>
#include <array>

class EBO {
public:
	inline EBO(GLuint handle) {
		this->handle = handle;
	};
	EBO() = default;

	static inline EBO make() {
		EBO ret;
		CHECK_GL(glGenBuffers(1, &ret.handle));
		return ret;
	};

	template <int AMOUNT>
	static inline std::array<EBO, AMOUNT> makeNum() {
		std::array<EBO, AMOUNT> rets;

		for (int i = 0; i < AMOUNT; i++)
			CHECK_GL(glGenBuffers(1, &rets[i].handle));

		return rets;
	};

	inline void bind() {
		CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
	};

	inline static void unbind() {
		CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	inline void fill(GLfloat* indices, int arrayLength, GLenum drawType) {
		GLint boundIndexBuffer;
		CHECK_GL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundIndexBuffer));
		
		if (handle != boundIndexBuffer) printf("[[WARNING]]: Trying to modify data of Index buffer %i from class of Index buffer: %u\n", boundIndexBuffer, handle);
		CHECK_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrayLength, (void*)indices, drawType));
	};

	inline void destroy() {
		GLint boundIndexBuffer;
		CHECK_GL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundIndexBuffer));

		if (boundIndexBuffer == handle) EBO::unbind();
		CHECK_GL(glDeleteBuffers(1, &handle));
	};

private:
	GLuint handle;
};