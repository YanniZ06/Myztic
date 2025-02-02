#pragma once

#include <ErrorHandler.hpp>

class Shader {
public:
	Shader() = default;
	/**
	 * Creates a shader with desired shader type and file location (file path, name and extension).
	 * 
	 * \param file file location information.
	 */
	Shader(GLenum shaderType, std::string file);

	/**  
	* Creates a shader with a desired shader type and the shader code.
	* \param shaderType The type of the shader (Vertex, Geometry, Fragment).
	* \param data The human readable shader code.
	*/
	static Shader fromString(GLenum shaderType, const char* data);

	inline void deleteShader() {
		CHECK_GL(glDeleteShader(handle));
	}

	GLuint handle;
};