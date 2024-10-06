#pragma once

#include <glad.h>
#include <exception>
#include <string>

static void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    bool dirty = false;
    while (err != GL_NO_ERROR)
    {
        const char* errorType = NULL; 
        switch (err) {
            case GL_INVALID_ENUM: errorType = "GL INVALID ENUM"; break; 
            case GL_INVALID_VALUE: errorType = "GL INVALID VALUE"; break; 
            case GL_INVALID_OPERATION: errorType = "GL INVALID OPERATION"; break; 
            case GL_INVALID_FRAMEBUFFER_OPERATION: errorType = "GL INVALID FRAMEBUFFER OPERATION"; break; 
            case GL_OUT_OF_MEMORY: errorType = "GL OUT OF MEMORY"; break; 
            default: errorType = "UNKNOWN ERROR."; break; 
        }; 

        printf("OpenGL error %s, at %s:%i - for %s\n", errorType, fname, line, stmt);
        err = glGetError();
        dirty = true;
    }

    if (dirty)
        abort();
}

#ifdef _DEBUG
#define CHECK_GL(stmt) \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__);
#else
#define CHECK_GL(stmt) stmt
#endif

class GLException : std::exception {
public:
    GLException(const char* msg) { m_msg = std::string(msg); };

    virtual const char* what() const throw ()
    {
        return m_msg.c_str();
    }

    std::string m_msg;
};