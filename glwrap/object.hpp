#pragma once

#include "glwrap/include_gl.h"

namespace glwrap
{

template <GLenum _binding>
class Object
{
  protected:
    GLuint m_handle;

  public:
    static inline GLenum BINDING = _binding;

    inline GLuint Handle() const { return m_handle; }

    /// @brief Gets the handle of the currently bound object
    static GLint GetBound()
    {
        GLint handle;
        glGetIntegerv(BINDING, &handle);
        return handle;
    }

    /// @brief Returns whether the object is currently bound
    inline bool IsBound() const { return GetBound() == m_handle; }
};

} // namespace glwrap
