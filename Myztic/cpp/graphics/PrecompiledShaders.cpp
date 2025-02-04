#include <pch.h>

#include <Timer.h>
#include <graphics\PrecompiledShaders.h>

Myztic::Shader Myztic::PrecompiledShaders::texture_color_vs = Shader();
Myztic::Shader Myztic::PrecompiledShaders::texture_color_fs = Shader();
Myztic::Shader Myztic::PrecompiledShaders::texture_vs = Shader();
Myztic::Shader Myztic::PrecompiledShaders::texture_fs = Shader();

void Myztic::PrecompiledShaders::Compile() {
	double start = Timer::stamp();
	texture_vs = Shader::fromString(GL_VERTEX_SHADER, BASIC_TEXTURE_VS);
	texture_color_vs = Shader::fromString(GL_VERTEX_SHADER, BASIC_TEXTURE_COLOR_VS);
	texture_fs = Shader::fromString(GL_FRAGMENT_SHADER, BASIC_TEXTURE_FS);
	texture_color_fs = Shader::fromString(GL_FRAGMENT_SHADER, BASIC_TEXTURE_COLOR_FS);
	Timer::debugMeasure(start, "Precompiled shaders");
};