#pragma once

namespace Myztic {
	typedef int fpsSize;

	class Fps {
		friend class Application;

	public:
		// Creates a new Fps Handler
		Fps(fpsSize max);
		// Default Constructor, use with max argument instead
		Fps() = default;
		// Destroys the Fps Handler
		//~Fps();


		//Gets the max fps cap
		inline fpsSize getMax() {
			return max;
		}
		// Sets the max fps cap
		void setMax(fpsSize max);

		// MS between each frame at max framerate
		inline double getFrameTime() {
			return frameTime;
		}
		inline float getRawFrameTime() {
			return rawFrameTime;
		}
		// The fps the app actually runs at currently
		inline fpsSize used() {
			return _used; // This value is populated by the Application
		}

	protected:
		fpsSize _used;
		fpsSize max;

		double frameTime;
		float rawFrameTime;
	};
}