#include <pch.h>

#include <graphics\Drawable.h>
#include <display\Window.h>
#include <graphics\Vertex.h>
#define STB_IMAGE_IMPLEMENTATION
#include <graphics\Sprite.h>

Sprite::Sprite(Window* drawerWin, VertexBuffer& vbuf, std::string texturePath) : Drawable(drawerWin, vbuf) {
	vert_type = GL_TRIANGLES;
	vbo.bind();
	//reconsider this especially for GL_STATIC_DRAW.
	vbo.fill((void*)vbuf.GetData(), vbuf.SizeBytes(), GL_STATIC_DRAW);

	inputLayout.setVertexLayout();
	inputLayout.enableAllAttribs();

	useEBO();

	vert_indices = std::vector<GLuint>({ 0, 1, 3, 1, 2, 3 });

	ebo.bind();
	ebo.fill(vert_indices.data(), sizeof(GLuint) * vert_indices.size(), GL_STATIC_DRAW);

	Shader vs = Shader(GL_VERTEX_SHADER, "assets/shaders/vs.glsl");
	Shader fs = Shader(GL_FRAGMENT_SHADER, "assets/shaders/fs.glsl");
	shaderProgram.attach(vs);
	shaderProgram.attach(fs);
	shaderProgram.link();
	vs.deleteShader();
	fs.deleteShader();

	inputLayout.unbind();
	vbo.unbind();
	texture = Texture2D::fromFile(texturePath);
}

void Sprite::prepareDraw()
{
	shaderProgram.bind();
	inputLayout.bindInputLayout();
	texture.bind();
}

void Sprite::endDraw()
{
	shaderProgram.unbind();
	inputLayout.unbind();
	texture.unbind();
}