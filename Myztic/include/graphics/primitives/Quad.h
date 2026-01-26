#pragma once

#include <graphics/PrecompiledShaders.h>

namespace Myztic 
{
	class Drawable;
	class Window;

	class Quad : public Drawable
	{
	public:
		/**
		 * Instantiates a quad and returns its pointer.
		 * 
		 * \param linkedScene The scene in which the quad exists
		 * \param x The x position of the top-left vertex
		 * \param y The y position of the top-left vertex
		 * \param width The width of the quad
		 * \param height The height of the quad - NOTE: if the quad will be bound to an orthographic plane (2D PLANE), PLEASE have this as a negative value so that x and y may be that of the top left corner of the quad (if you have this as a positive value then x and y concern the bottom left vertex)
		 * \param z The z position of the top-left vertex (and consequently all the other vertices because I (Ziad, Zoardedz (that sounded narcissitic)) made it that way because it would be stupid design.)
		 * \param color The color the quad should have.
		 * \return 
		 */
		static Quad* makeQuad(Scene* linkedScene, float x, float y, float width, float height, float z = 0, glm::vec4 color = glm::vec4(1.f,1.f,1.f,1.f));

		/**
		 * The constructor of a quad, use this if you want to independently feed in your own vertex buffer and other variables independent of the standard/unmanipulated definition.
		 * 
		 * \param linkedScene The scene in which the quad exists
		 * \param vbuf The vertex buffer object containing the vertices' data
		 * \param x The x coordinate of the top-left vertex
		 * \param y The y coordinate of the top-left vertex
		 * \param width The width of the quad
		 * \param height The height of the quad - NOTE: if the quad will be bound to an orthographic plane (2D PLANE), PLEASE have this as a negative value so that x and y may be that of the top left corner of the quad (if you have this as a positive value then x and y concern the bottom left vertex)
		 * \param z The z position of the top-left vertex (and consequently all the other vertices because I  made it that way because it would be stupid design.)
		 * \param shaders The shaders that should be input into the shader program regarding the quad (this displays the pixels)
		 * \param color The color of the quad
		 */
		Quad(Scene* linkedScene, VertexBuffer& vbuf, float x, float y, float width, float height, float z = 0, std::vector<Shader> shaders = {PrecompiledShaders::color_vs, PrecompiledShaders::color_fs}, glm::vec4 color = glm::vec4(1.f,1.f,1.f,1.f));
	
		//Retrieves the x coordinate of the top-left vertex
		float get_x();
		//Retrieves the y coordinate of the top-left vertex
		float get_y();
		//Retrieves the z coordinate of the quad
		float get_z();
		//Retrieves the width of the quad
		float get_width();
		//Retrieves the height of the quad
		float get_height();
		//Retrieves the color of the quad
		glm::vec4 get_color();

		/**
		 * Sets the x coordinate of the top-left vertex.
		 * 
		 * \param x The new x coordinate
		 */
		void set_x(float x);
		/**
		 * Sets the y coordinate of the top-left vertex.
		 * 
		 * \param y The new y coordinate
		 */
		void set_y(float y);
		/**
		 * Sets the z coordinate of the quad.
		 * 
		 * \param z The new z coordinate
		 */
		void set_z(float z);
		/**
		 * Sets the width of the quad.
		 * 
		 * \param width The new width
		 */
		void set_width(float width);
		/**
		 * Sets the height of the quad (be wary of usage in orthographic/2D planes).
		 * 
		 * \param height The new height of the quad - NOTE: if the quad will be bound to an orthographic plane (2D PLANE), PLEASE have this as a negative value so that x and y may be that of the top left corner of the quad (if you have this as a positive value then x and y concern the bottom left vertex)
		 */
		void set_height(float height);
		/**
		 * Sets the color of the quad.
		 * 
		 * \param color The new color
		 */
		void set_color(glm::vec4 color);
	private:
		float x;
		float y;
		float z;
		float width;
		float height;
		glm::vec4 color;
	};
}