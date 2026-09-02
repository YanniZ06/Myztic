#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
void main()
{
    FragColor = (texture(texture_diffuse1, TexCoord) * texture(texture_diffuse2, TexCoord) * texture(texture_diffuse3, TexCoord)) + (texture(texture_specular1, TexCoord) * texture(texture_specular2, TexCoord));
}