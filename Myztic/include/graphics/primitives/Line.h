#pragma once

#include <graphics/PrecompiledShaders.h>

namespace Myztic {
	class Drawable;
	class Window;

	class Line : public Drawable {
	public:
		/* Instantiates a new Line object (2D object visible in 2D space which sticks to your camera everywhere you go) whose starting point is "startpoint" and ending point is "endpoint"
		* \param linkedScene The scene in which the line should be bound to
		* \param startpoint The startpoint of the vector/line
		* \param endpoint The endpoint of the vector/line
		* \param color The color of the line
		* \return Line* The line object
		*/
		static Line* createLine(Scene* linkedScene, glm::vec2 startpoint, glm::vec2 endpoint, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f));
		/**
		* Instantiates a new Line object (3D object visible in 3D space) whose starting point is "startpoint" (3D Vector) and ending point is "endpoint"
		* \param linkedScene The scene in which the ray should be bound to
		* \param startpoint The startpoint of the vector/ray
		* \param endpoint The startpoint of the vector/ray
		* \param color The color of the line
		* \return Line* The ray object 
		*/
		static Line* createRay(Scene* linkedScene, glm::vec3 startpoint, glm::vec3 endpoint, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f));
		
		/**
		* Constructor for the line, this may be used directly if createLine and createRay aren't desired or if you want to implement your custom shader or vertex buffer.
		* 
		* \param linkedScene The scene in which the line should be bound to
		* \param vbuf The vertex buffer object that should be input; general layout should have Position3D and Float4Color
		* \param startpoint The startpoint of the vector/line
		* \param endpoint The endpoint of the vector/line
		* \param color The color the line should be (used for data storage and color mixing is disallowed/not supported; however, the initial vertexbuffer object can be fed varying colors however you desire (changing the color using set_color changes that completely)
		* \param shaders The shaders that should be input into the shader program of the line
		*/
		Line(Scene* linkedScene, VertexBuffer& vbuf, glm::vec3 startpoint, glm::vec3 endpoint, glm::vec4 color, std::vector<Shader> shaders = {PrecompiledShaders::line_vs, PrecompiledShaders::line_fs});

		/*
		* \returns The startpoint of the vector/line
		*/
		glm::vec3 get_startpoint();

		/*
		* \returns The endpoint of the vector/line
		*/
		glm::vec3 get_endpoint();

		/*
		* \returns The color of the line
		*/
		glm::vec4 get_color();

		/**Sets the startpoint of the line to the new startpoint
		* \param new_startpoint The new startpoint of the vector/line
		* \returns void
		*/
		void set_startpoint(glm::vec3 new_startpoint);

		/* Sets the endpoint of the line to the new endpoint
		* \param new_endpoint The new endpoint of the vector/line
		* \returns void
		*/
		void set_endpoint(glm::vec3 new_endpoint);

		/*
		* \param new_color The new color the line should be
		* \returns void
		*/
		void set_color(glm::vec4 new_color);
	private:
		glm::vec3 u_start;
		glm::vec3 u_end;
		glm::vec4 u_color;
	};
}