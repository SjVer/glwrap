#include <glad/glad.h>

namespace glwrap
{

template <GLenum binding = 0>
class Resource
{
  protected:
    GLuint m_handle;

  public:
    static inline GLenum BINDING = binding;

    inline GLuint Handle() const { return m_handle; }

    /// @brief Gets the handle of the currently bound resource
    static GLint GetBound()
    {
        GLint handle;
        glGetIntegerv(binding, &handle);
        return handle;
    }
};

} // namespace glwrap