#pragma once

#include <glad.h>
#include <exception>
#include <string>

static void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        abort();
    }
}

#ifdef _DEBUG
#define CHECK_GL(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
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