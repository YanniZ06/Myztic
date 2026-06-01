#pragma once
#include <graphics/PrecompiledShaders.h>

namespace Myztic {
	class Window;
	class TexturedDrawable;
	
	class Cube : public TexturedDrawable {
	public:
		/**
		 * Instantiates a cube and returns its pointer.
		 *
		 * \param linkedScene The scene in which the cube exists
		 * \param x The x position of the first vertex
		 * \param y The y position of the first vertex
		 * \param z The z position of the first
		 * \param size The side length of the cube in units
		 * \param color The color the cube should be.
		 * \return Cube*
		 */
		static Cube* makeCube(Scene* linkedScene, float x, float y, float z, float size = 1.0f, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f), std::vector<Shader> shaders = { PrecompiledShaders::color_vs, PrecompiledShaders::color_fs });

		/**
		 * Instantiates a cube instance.
		 * 
		 * \param linkedScene The scene in which the cube should exist.
		 * \param vbuf The vertexbuffer of the cube (automatically passed through by makeCube)
		 * \param size The side length of the cube
		 * \param color The color of each vertex of the cube
		 * \param shaders The shaders that should be applied to the cube
		 */
		Cube(Scene* linkedScene, VertexBuffer& vbuf, float size = 1.f, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f), std::vector<Shader> shaders = {PrecompiledShaders::color_vs, PrecompiledShaders::color_fs});
	
		glm::vec4 get_color();
		void set_color(glm::vec4 new_col);
	private:
		glm::vec4 color;
		float size;
	};
}