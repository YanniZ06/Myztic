#pragma once

#include <ErrorHandler.hpp>
#include <map>

class Shader {
public:
	Shader(GLenum shaderType, std::string file);

	GLuint handle;
};

class ShaderProgram {
public:
	ShaderProgram();
	inline void link() {
		CHECK_GL(glLinkProgram(handle));
	};

	inline void attach(Shader shader) {
		CHECK_GL(glAttachShader(handle, shader.handle));
	};

	inline void bind() {
		CHECK_GL(glUseProgram(handle));
	};

	static inline void unbind() {
		CHECK_GL(glUseProgram(0));
	};

	inline void destroy() {
		CHECK_GL(glDeleteProgram(handle));
	};

	int getUniformLocation(std::string uniformName);
	void cacheUniform(std::string uniformName);
	
	GLuint handle;
	std::map<std::string, int> uniforms;
};