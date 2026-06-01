#include <pch.h>

#include <graphics\Drawable.h>
#include <display\Window.h>
#include <graphics\Vertex.h>
//this must only be called once and it must be in a source file not an hpp file like Texture2D
#define STB_IMAGE_IMPLEMENTATION
#include <graphics\TexturedDrawable.h>

using namespace Myztic;

TexturedDrawable::TexturedDrawable(Scene* linkedScene, VertexBuffer& vbuf, std::string texturePath) : Drawable(linkedScene, vbuf) {
	texture = Texture2D::fromFile(texturePath);
}

void TexturedDrawable::prepareDraw()
{
	Drawable::prepareDraw();
	if (texture.getHandle() != 0)
		texture.bind();
}

void TexturedDrawable::finishDraw()
{
	Drawable::finishDraw();
	if (texture.getHandle() != 0)
		texture.unbind();
}

void TexturedDrawable::loadTexture(std::string texturePath)
{
	texture = Texture2D::fromFile(texturePath);
}
