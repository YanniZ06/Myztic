#pragma once

#include <ErrorHandler.hpp>
#include <array>

class VAO {
public:
	inline VAO(GLuint handle = 0) { this->vao = handle; }
	/*
	* Makes a singular VAO.
	* 
	*/
	inline static VAO make() {
		//this uses the default constructor, that's why its required
		VAO ret = VAO(0);
		CHECK_GL(glGenVertexArrays(1, &ret.vao));
		return ret;
	};

	template <int AMOUNT>
	/*
	* Makes a number of VAOs, the amount of them should be passed in the template.
	* 
	*/
	inline static std::array<VAO, AMOUNT> makeNum() {
		//this uses the default constructor too !!!
		std::array<VAO, AMOUNT> rets = std::array<VAO, AMOUNT>();

		for (int i = 0; i < AMOUNT; i++)
			CHECK_GL(glGenVertexArrays(1, &rets[i].vao));
		
		return rets;
	};

	/*
	* Unbinds the VAO from the OpenGL pipeline.
	* 
	*/
	inline static void unbind() {
		CHECK_GL(glBindVertexArray(0));
	};

	/*
	* Binds the VAO to the OpenGL pipeline.  
	* 
	*/
	inline void bind() {
		CHECK_GL(glBindVertexArray(vao));
	};

	/* 
	* Deletes the VAO and deconstructs it.  
	* 
	*/
	inline void destroy() {
		int buf;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &buf);

		if (buf == vao) {
			printf("[[WARNING]]: TRYING TO DESTROY THE CURRENTLY BOUND VERTEX ARRAY OBJECT: %i, UNBINDING OBJECT\n", buf);
			VAO::unbind();
		}

		CHECK_GL(glDeleteVertexArrays(1, &vao));
	};
	GLuint vao;
private:

};