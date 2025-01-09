#pragma once

#include <ErrorHandler.hpp>

class Shader {
public:
	/**
	 * Creates a shader with desired shader type and file location (file path, name and extension).
	 * 
	 * \param file file location information.
	 */
	Shader(GLenum shaderType, std::string file);

	inline void deleteShader() {
		CHECK_GL(glDeleteShader(handle));
	}

	GLuint handle;
};