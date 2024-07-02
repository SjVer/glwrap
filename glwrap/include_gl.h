#pragma once

#ifdef GLWRAP_INCLUDE_CUSTOM_GL
#include "glwrap_include_gl.h"

#elif !defined(GLWRAP_GL_ALREADY_INCLUDED)

#if defined(GLWRAP_INCLUDE_GLAD) || true
#include <glad/glad.h>
#endif

#endif