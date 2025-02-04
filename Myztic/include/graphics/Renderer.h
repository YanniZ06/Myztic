#pragma once

#include <map>
#include <graphics/Drawable.h>
#include <glm\glm.hpp>
#include <type_traits>
#include "SDL.h"

// todo: seperate these classes accordingly

// todo: create different types of render requests, make them work more like enums yadda yadda, find out if we even need a request stack
// maybe we can just evaluate all we need from the drawable at the end of the update loop inside the rendering thread, instead of sending requests and then reading them
// at about the same time?? sending requests  doesnt seem extremely smart unless we send them directly without a drawable object related (aka tell renderer to draw a line etc,)
namespace Myztic {
	class Window;
	/**
	 * Represents a type of request to the renderer.
	 *
	 * Prefixes are as follow:
	 * **D_** is used for drawing requests, such as drawing a triangle.
	 * **
	 */
	enum RenderRequestType {
		D_LINE, D_TRI, D_QUAD,
		D_RAY
	};
	//above feels odd, i wanna be just passing in a drawable and then it immediately renders, feels overcomplicated YANNI

	class RenderRequestBase {
	public:
		RenderRequestBase() = default;
		inline RenderRequestType type() { return _type; }
		inline RenderRequestBase base() { return *this; }
	protected:
		RenderRequestType _type;
	};

	/**
	 * A basic request to draw a line from 2 points.
	 * Also handles 3D line (ray) drawing depending on if vec2 or vec3 is passed in.
	 */
	template<class VecCount>
	class DLineRequest : public RenderRequestBase {
	public:
		VecCount p1, p2;

		DLineRequest(VecCount p1, VecCount p2) {
			this->p1 = p1;
			this->p2 = p2;

			if constexpr (std::is_same<VecCount, glm::vec2>()) _type = D_LINE;
			else _type = D_RAY;
		}
		DLineRequest() = default;

		// Shouldnt the compiler get rid of all of these on its own???
		//~DLineRequest() {
		//};
	};


	/*struct RenderRequestStack {
		///
		std::vector<RenderRequestBase> stack;
		/// The amount of requests currently in this stack.
		// long size;
	};*/

	class Renderer {
		friend class Application;
	public:
		Renderer(Window* drawTarget);
		Renderer() = default;
		~Renderer();

		/// Render requests to be handled the next frame.
		std::vector<RenderRequestBase> pendingRequests; //! Protected????
		std::vector<Drawable*> drawables;

		/**
		 * Requests the renderer to draw a basic 2D line from 2 points.
		 *
		 * \param p1 Point A
		 * \param p2 Point B
		 * \see drawRay
		 */
		void drawLine(glm::vec2 p1, glm::vec2 p2);

		/**
		 * Requests the renderer to draw a basic 3D line from 2 points.
		 *
		 * \param p1 Point A
		 * \param p2 Point B
		 * \see drawLine
		 */
		void drawRay(glm::vec3 p1, glm::vec3 p2);
	protected:
		/**
		* Handles rendering and presenting a frame to the screen.
		*
		*/
		void startRender();

		Window* targetWin;

		/**
		* Presents the drawn objects to the screen.
		*
		*/
		void endRender();

		/// Render requests to be handled this frame.
		std::vector<RenderRequestBase> nextRequests;
	};
}