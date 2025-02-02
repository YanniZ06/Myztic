#include "pch.h"
#include <graphics/backend/Shader.h>
#include <fstream>

Shader::Shader(GLenum shaderType, std::string file) {
	std::ifstream shaderFile(file, std::ios::in | std::ios::binary | std::ios::ate);

	if (shaderFile.is_open()) {
		shaderFile.seekg(0, std::ios::end);
		int size = shaderFile.tellg();
		char* content = new char[size];
		shaderFile.seekg(0, std::ios::beg);
		shaderFile.read(content, size);

		//Do our own tomfuckery
		handle = CHECK_GL(glCreateShader(shaderType));
		CHECK_GL(glShaderSource(handle, 1, &content, &size));
		CHECK_GL(glCompileShader(handle));

		GLint result;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &result);

		if (!result) {
			int length;
			CHECK_GL(glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length));

			// + 1 for null terminator
			GLchar* infoLog = new GLchar[length + (size_t)1];
			CHECK_GL(glGetShaderInfoLog(handle, length, nullptr, infoLog));

			std::string log(infoLog);

			delete[] infoLog;
			throw "Could not compile shader, info dump: " + log;
		}

		shaderFile.clear();
		shaderFile.close();

		delete[] content;

		return;
	};

	printf("Failed to open shader file: %s\n", file.c_str());

	throw "Couldn't open the shader file.";
};

Shader Shader::fromString(GLenum shaderType, const char* data) {
	Shader ret = Shader();
	ret.handle = CHECK_GL(glCreateShader(shaderType));
	CHECK_GL(glShaderSource(ret.handle, 1, &data, NULL));
	CHECK_GL(glCompileShader(ret.handle));

	GLint result;
	glGetShaderiv(ret.handle, GL_COMPILE_STATUS, &result);

	if (!result) {
		int length;
		CHECK_GL(glGetShaderiv(ret.handle, GL_INFO_LOG_LENGTH, &length));

		// + 1 for null terminator
		GLchar* infoLog = new GLchar[length + (size_t)1];
		CHECK_GL(glGetShaderInfoLog(ret.handle, length, nullptr, infoLog));

		std::string log(infoLog);

		delete[] infoLog;
		throw "Could not compile shader, info dump: " + log;
	}

	return ret;
};