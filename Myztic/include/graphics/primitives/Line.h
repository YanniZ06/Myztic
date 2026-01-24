#pragma once

#include <graphics/Drawable.h>

namespace Myztic {
	class Drawable;
	class Window;

	class Line : public Drawable {
	public:
		/* Instantiates a new Line object (2D object visible in 2D space which sticks to your camera everywhere you go) whose starting point is "startpoint" and ending point is "endpoint"
		* \param linkedScene The scene in which the line should be bound to
		* \param startpoint The startpoint of the vector/line
		* \param endpoint The endpoint of the vector/line
		* \return Line* The line object
		*/
		static Line* createLine(Scene* linkedScene, glm::vec2 startpoint, glm::vec2 endpoint);
		/**
		 * Instantiates a new Line object (3D object visible in 3D space) whose starting point is "startpoint" (3D Vector) and ending point is "endpoint"
		 * \param linkedScene The scene in which the ray should be bound to
		 * \param startpoint The startpoint of the vector/ray
		 * \param endpoint The startpoint of the vector/ray
		 * \return 
		 */
		Line* createRay(Scene* linkedScene, glm::vec3 startpoint, glm::vec3 endpoint);

		/*
		* \returns The startpoint of the vector/line
		*/
		glm::vec2 get_startpoint();

		/*
		* \returns The endpoint of the vector/line
		*/
		glm::vec2 get_endpoint();

		/**Sets the startpoint of the line to the new startpoint
		* \param new_startpoint The new startpoint of the vector/line
		* \returns void
		*/
		void set_startpoint(glm::vec2 new_startpoint);
		/* Sets the endpoint of the line to the new endpoint
		* \param new_endpoint The new endpoint of the vector/line
		* \returns void
		*/
		void set_endpoint(glm::vec2 new_endpoint);

	private:
		Line(Scene* linkedScene, VertexBuffer& vbuf);

		glm::vec2 u_start;
		glm::vec2 u_end;
	};
}