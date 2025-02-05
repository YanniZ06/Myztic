#include "pch.h"

#include <graphics/backend/ShaderProgram.h>

using namespace Myztic;

ShaderProgram::ShaderProgram() {
	handle = CHECK_GL(glCreateProgram());
	uniforms = std::map<std::string, int>();
};

int ShaderProgram::getUniformLocation(std::string uniformName) {
	if (!uniforms.count(uniformName))
		cacheUniform(uniformName);

	return uniforms[uniformName];
};

void ShaderProgram::cacheUniform(std::string uniformName) {
	GLint uniformLocation = CHECK_GL(glGetUniformLocation(handle, uniformName.c_str()));
	uniforms[uniformName] = uniformLocation;
};
