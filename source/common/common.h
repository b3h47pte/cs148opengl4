#pragma once

#ifndef __COMMON__
#define __COMMON__

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "GL/glew.h"

#ifdef __APPLE__
#include "OpenGL/gl.h"
#endif

#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <unordered_map>

inline std::string _OpenGLErrorToString(GLenum err) {
    // Error messages copied from the glGetError documentation: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
    switch (err) {
    case GL_NO_ERROR:
        return "No Error: No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
    case GL_INVALID_ENUM:
        return "Invalid enum: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag."; 
    case GL_INVALID_VALUE:
        return "Invalid value: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:
        return "Invalid operation: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "Invalid framebuffer operation: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:
        return "Out of memory: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    case GL_STACK_UNDERFLOW:
        return "Stack Underflow: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    case GL_STACK_OVERFLOW:
        return "Stack overflow: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    default:
        break;
    }
    return "Unspecified error";
}

// Error Detection for OpenGL
inline void _DisplayOpenGLError(std::string command, std::string file, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err << " -- " << _OpenGLErrorToString(err) << std::endl;
        std::cerr << "Relevant Command: " << command << std::endl;
        std::cerr << "Location: " << file << " at Line: " << line << std::endl;
    }
}

#ifndef NDEBUG
#define OGL_CALL(x) x; _DisplayOpenGLError(#x, __FILE__, __LINE__);
#else
#define OGL_CALL(x) x;
#endif

#endif
