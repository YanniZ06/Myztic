#include <pch.h>

#include <Timer.h>
#include <graphics\PrecompiledShaders.h>

using namespace Myztic;

Shader PrecompiledShaders::texture_color_vs = Shader();
Shader PrecompiledShaders::texture_color_fs = Shader();
Shader PrecompiledShaders::texture_vs = Shader();
Shader PrecompiledShaders::texture_fs = Shader();

void PrecompiledShaders::Compile() {
	double start = Timer::stamp();
	texture_vs = Shader::fromString(GL_VERTEX_SHADER, BASIC_TEXTURE_VS);
	texture_color_vs = Shader::fromString(GL_VERTEX_SHADER, BASIC_TEXTURE_COLOR_VS);
	texture_fs = Shader::fromString(GL_FRAGMENT_SHADER, BASIC_TEXTURE_FS);
	texture_color_fs = Shader::fromString(GL_FRAGMENT_SHADER, BASIC_TEXTURE_COLOR_FS);
	Timer::debugMeasure(start, "Precompiled shaders");
};