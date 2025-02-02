#pragma once

#include <graphics\backend\Shader.h>

#define BASIC_TEXTURE_COLOR_VS R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aCol;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

out vec4 aColor;
out vec2 TexCoord;
void main()
{
    gl_Position = projection * view * world * vec4(aPos, 1.0);
    aColor = aCol;
    TexCoord = aTexCoord;
})"

#define BASIC_TEXTURE_COLOR_FS R"(#version 330 core
out vec4 FragColor;
in vec4 aColor;
in vec2 TexCoord;
uniform sampler2D tex;
void main()
{
    FragColor = texture(tex, TexCoord) * aColor;
})"

#define BASIC_TEXTURE_VS R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
void main()
{
    gl_Position = projection * view * world * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
})"

#define BASIC_TEXTURE_FS R"(#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D tex;
void main()
{
    FragColor = texture(tex, TexCoord);
})"

class PrecompiledShaders {
public:
    static Shader texture_color_vs;
    static Shader texture_color_fs;
    static Shader texture_vs;
    static Shader texture_fs;

    static void Compile();
};