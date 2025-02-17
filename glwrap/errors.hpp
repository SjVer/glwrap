#pragma once

#include "glwrap/include_gl.h"

namespace glwrap
{

/// @brief Gets a string representation of a GL error
static const char* GetErrorString(GLenum error)
{
    switch (error)
    {
        case GL_NO_ERROR:                      return "GL_NO_ERROR";
        case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
        default:                               return nullptr;
    }
}

} // namespace glwrap