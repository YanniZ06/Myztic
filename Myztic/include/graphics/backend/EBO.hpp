#pragma once

#include <ErrorHandler.hpp>
#include <array>

class EBO {
public:
	inline EBO(GLuint handle) {
		this->handle = handle;
	};
	EBO() = default;

	/**
	 * Creates a single EBO instance
	 * 
	 */
	static inline EBO make() {
		EBO ret;
		CHECK_GL(glGenBuffers(1, &ret.handle));
		return ret;
	};

	template <int AMOUNT>
	/**
	 * Creates multiple EBO instances
	 * 
	 */
	static inline std::array<EBO, AMOUNT> makeNum() {
		std::array<EBO, AMOUNT> rets;

		for (int i = 0; i < AMOUNT; i++)
			CHECK_GL(glGenBuffers(1, &rets[i].handle));

		return rets;
	};

	/**
	 * Binds the EBO to the OpenGL pipeline
	 * 
	 */
	inline void bind() {
		CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
	};

	/**
	 * Unbinds the EBO from the OpenGL pipeline
	 * 
	 */
	inline static void unbind() {
		CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	/**
	 * Fills the EBO with the desired data
	 * 
	 * \param indices The array of elements/indices being passed in.
	 * \param arrayLength The length of the array being passed in.
	 * \param drawType Type of draw sequence. (static or dynamic or otherwise)
	 */
	inline void fill(GLfloat* indices, int arrayLength, GLenum drawType) {
		GLint boundIndexBuffer;
		CHECK_GL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundIndexBuffer));
		
		if (handle != boundIndexBuffer) printf("[[WARNING]]: Trying to modify data of Index buffer %i from class of Index buffer: %u\n", boundIndexBuffer, handle);
		CHECK_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrayLength, (void*)indices, drawType));
	};

	/**
	 * Destroys the EBO and deconstructs.
	 * 
	 */
	inline void destroy() {
		GLint boundIndexBuffer;
		CHECK_GL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundIndexBuffer));

		if (boundIndexBuffer == handle) EBO::unbind();
		CHECK_GL(glDeleteBuffers(1, &handle));
	};

private:
	GLuint handle;
};