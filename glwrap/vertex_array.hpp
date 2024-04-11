#include <glad/glad.h>

#include "glwrap/resource.hpp"

namespace glwrap
{

/**
 * @brief A vertex array object
 */
class VertexArray : public Resource<GL_VERTEX_ARRAY_BINDING>
{
  public:
    VertexArray() { glGenVertexArrays(1, &m_handle); }
    ~VertexArray() { glDeleteVertexArrays(1, &m_handle); }

    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other) = delete;

    /// @brief Gets a handle to the buffer bound to the vertex array
    GLint BufferHandle() const
    {
        GLint handle;
        glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &handle);
        return handle;
    }

    void Bind() const { glBindVertexArray(m_handle); }
    void Unbind() const { glBindVertexArray(0); }
};

} // namespace glwrap