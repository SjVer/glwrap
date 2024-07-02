#pragma once

#include "glwrap/include_gl.h"

#ifndef GL_VERSION_3_0
#error "OpenGL 3.0 is required to use VertexArray"
#endif

#include "glwrap/object.hpp"

namespace glwrap
{

/**
 * @brief A vertex array object
 */
class VertexArray : public Object<GL_VERTEX_ARRAY_BINDING>
{
  protected:
    inline void BindIfUnbound() const
    {
        if (!IsBound()) Bind();
    }

  public:
    VertexArray() { glGenVertexArrays(1, &m_handle); }
    ~VertexArray() { glDeleteVertexArrays(1, &m_handle); }

    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other) = delete;

    void Bind() const { glBindVertexArray(m_handle); }
    void Unbind() const { glBindVertexArray(0); }

    /**
     * @brief Defines a vertex attribute
     * @see glVertexAttribPointer
     *
     * @param index The index of the attribute
     * @param components The number of components per vertex
     * @param type The OpenGL data type of each component
     * @param normalized Whether the data should be normalized
     * @param stride The byte offset between consecutive attributes
     * @param offset The byte offset of the first component
     */
    void DefineAttribute(
        GLuint index, GLint components, GLenum type,
        GLboolean normalized, GLsizei stride, size_t offset
    )
    {
        BindIfUnbound();
        glVertexAttribPointer(
            index, components, type,
            normalized, stride, reinterpret_cast<void*>(offset)
        );
    }
};

} // namespace glwrap