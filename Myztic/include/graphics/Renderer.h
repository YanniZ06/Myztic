#pragma once

#include <map>
#include <graphics/Drawable.h>

// todo: create different types of render requests, make them work more like enums yadda yadda, find out if we even need a request stack
// maybe we can just evaluate all we need from the drawable at the end of the update loop inside the rendering thread, instead of sending requests and then reading them
// at about the same time?? sending requests  doesnt seem extremely smart unless we send them directly without a drawable object related (aka tell renderer to draw a line etc,)
struct RenderRequest {
	Drawable* drawObj;
};
struct RenderRequestStack {
	/// 
	RenderRequest* stack;
	/// The amnount of requests currently in this stack.
	long size;
};
class Renderer {
public:
	static std::map<Window*, RenderRequestStack> pendingRequests;
};
