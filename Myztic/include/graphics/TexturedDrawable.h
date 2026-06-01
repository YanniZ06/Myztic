#pragma once

#include <graphics\backend\Texture2D.hpp>
#include <vector>
#include <graphics\PrecompiledShaders.h>

namespace Myztic {
	class Drawable;
	class Window;
	class TexturedDrawable : public Drawable {
	public:
		TexturedDrawable(Scene* linkedScene, VertexBuffer& vbuf, std::string texturePath = "");
		void prepareDraw();
		void finishDraw();

		//overrides current texture! (must provide an assets system later on that indexes each asset separately so that it may be reused/recycled instead of being bound to every object separately.
		void loadTexture(std::string texturePath);
	private:
		Texture2D texture;
	};
}