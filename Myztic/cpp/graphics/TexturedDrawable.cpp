#include <pch.h>

#include <graphics\Drawable.h>
#include <display\Window.h>
#include <graphics\Vertex.h>
//this must only be called once and it must be in a source file not an hpp file like Texture2D
#define STB_IMAGE_IMPLEMENTATION
#include <graphics\TexturedDrawable.h>

Myztic::TexturedDrawable::TexturedDrawable(Scene* linkedScene, VertexBuffer& vbuf, std::string texturePath, bool usesEBO, std::vector<Shader> shaders) : Drawable(linkedScene, vbuf) {
	vert_type = GL_TRIANGLES;
	vbo.bind();
	//reconsider this especially for GL_STATIC_DRAW.
	vbo.fill((void*)vbuf.GetData(), vbuf.SizeBytes(), GL_STATIC_DRAW);

	inputLayout.setVertexLayout();
	inputLayout.enableAllAttribs();

	if (usesEBO)
		useEBO();

	if (usesEBO) {
		vert_indices = std::vector<GLuint>({ 0, 1, 3, 1, 2, 3 });
		ebo.bind();
		ebo.fill(vert_indices.data(), sizeof(GLuint) * vert_indices.size(), GL_STATIC_DRAW);
	}

	for (Shader s : shaders)
		shaderProgram.attach(s);

	shaderProgram.link();

	inputLayout.unbind();
	vbo.unbind();
	texture = Texture2D::fromFile(texturePath);
}

void Myztic::TexturedDrawable::prepareDraw()
{
	Drawable::prepareDraw();
	texture.bind();
}

void Myztic::TexturedDrawable::finishDraw()
{
	Drawable::finishDraw();
	texture.unbind();
}