#pragma once

#include <graphics\backend\Texture2D.hpp>

class Drawable;
class Window;

class Sprite : public Drawable {
public:
	Sprite(Window* drawerWin, VertexBuffer& vbuf, std::string texturePath);

	virtual void prepareDraw();
	virtual void endDraw();
private:
	Texture2D texture;
};