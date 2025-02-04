#pragma once

#include <graphics\backend\Texture2D.hpp>
#include <vector>
#include <graphics\PrecompiledShaders.h>

namespace Myztic {
	class Drawable;
	class Window;
	class TexturedDrawable : public Drawable {
	public:
		TexturedDrawable(Scene* linkedScene, VertexBuffer& vbuf, std::string texturePath, bool usesEBO = false, std::vector<Shader> shaders = { PrecompiledShaders::texture_vs, PrecompiledShaders::texture_fs });
		void prepareDraw();
		void finishDraw();
	private:
		Texture2D texture;
	};
}