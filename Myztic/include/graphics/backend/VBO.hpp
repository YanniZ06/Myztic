#pragma once

#include <ErrorHandler.hpp>
#include <array>
#include <graphics\backend\ShaderInputLayout.h>
#include <glm.hpp>
#include <graphics\Vertex.h>

namespace Myztic {
	class VBO {
	public:
		inline VBO(GLuint vbo) { this->vbo = vbo; };
		inline VBO() = default;

		/*
		* Makes a singular VBO.
		*
		*/
		inline static VBO make() {
			VBO ret;
			CHECK_GL(glGenBuffers(1, &ret.vbo));
			return ret;
		};

		template <int AMOUNT>
		/*
		* Makes a number of VBOs, the amount of them should be passed in the template.
		*
		*/
		inline static std::array<VBO, AMOUNT> makeNum() {
			std::array<VBO, AMOUNT> rets;

			for (int i = 0; i < AMOUNT; i++)
				CHECK_GL(glGenBuffers(1, &rets[i].vbo));

			return rets;
		};

		/*
		* Unbinds the vertex buffer from the OpenGL pipeline
		*
		*/
		inline static void unbind() {
			CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		};

		/*
		* Binds the vertex buffer to the OpenGL pipeline
		*
		*/
		inline void bind() {
			CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		};

		/*
		* Deletes the vertex buffer and deconstructs the class
		*
		*/
		inline void destroy() {
			int buf;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buf);

			if (buf == vbo) {
				printf("[[WARNING]]: TRYING TO DESTROY THE CURRENTLY BOUND BUFFER: %i, UNBINDING BUFFER\n", buf);
				VBO::unbind();
			}

			CHECK_GL(glDeleteBuffers(1, &vbo));
		};

		/*
		* Fills the vertex buffer with information
		*
		* \param data The data to input into the vertex buffer
		* \param arrayLength The length of the array of elements to fill the vertex buffer
		* \param drawType The drawing method the renderer should use, should use GL_STATIC_DRAW if you aren't gonna modify the vertices and use GL_DYNAMIC_DRAW if you will.
		*/
		inline void fill(void* data, size_t arrayLength, GLenum drawType) {
			int buf;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buf);

			if (buf != vbo)
				printf("[[WARNING]]: TRYING TO MODIFY CURRENTLY BOUND VERTEX BUFFER: %i FROM A NOT BOUND BUFFER CLASS: %u\n", buf, vbo);

			CHECK_GL(glBufferData(GL_ARRAY_BUFFER, arrayLength, data, drawType));
		};

		/**
		 * Updates the currently bound vertex buffer with the provided data; requires the vertex buffer to have GL_DYNAMIC_DRAW input when it was first written to.
		 * 
		 * \param data The new data to replace previous data
		 * \param offset The offset in the previous data array from which you want to start writing
		 * \param size Specifies the size in bytes of the data store region being replaced. 
		 */
		inline void update(void* data, size_t offset, size_t size) {
			int buf;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buf);

			if (buf != vbo)
				printf("[[WARNING]]: TRYING TO MODIFY CURRENTLY BOUND VERTEX BUFFER: %i FROM A NOT BOUND BUFFER CLASS: %u\n", buf, vbo);

			CHECK_GL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
		};
	private:
		GLuint vbo;
	};
}