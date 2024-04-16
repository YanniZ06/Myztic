#pragma once

#include <Myztic.h>
#include <glad/glad.h>
#include <string>

#define CHECK_GL() GLint result = glGetError(); \
	if (!result) return NULL; \
	while (result) { \
		const char* errorType = NULL;\
		switch(result) {\
		case GL_INVALID_ENUM: errorType = "GL INVALID ENUM"; break;\
		case GL_INVALID_VALUE: errorType = "GL INVALID VALUE"; break;\
		case GL_INVALID_OPERATION: errorType = "GL INVALID OPERATION"; break;\
		case GL_INVALID_FRAMEBUFFER_OPERATION: errorType = "GL INVALID FRAMEBUFFER OPERATION"; break;\
		case GL_OUT_OF_MEMORY: errorType = "GL OUT OF MEMORY"; break;\
		default: errorType = "UNKNOWN ERROR."; break;\
		};\
		printf("Got a GL error on line: %i from file: %s\nError type: %s\n", __LINE__, __FILE__, errorType);\
		\
		result = glGetError();\
	}; \
	throw "GL Errors traced, throwing to stop application"; \