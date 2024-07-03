#pragma once

#include "glwrap/include_gl.h"
#include "glwrap/object.hpp"

namespace glwrap
{

/**
 * @brief A texture object
 *
 * @tparam _target The target of the texture
 * @tparam _binding The binding point of the texture
 */
template <GLenum _target, GLenum _binding>
class Texture : public Object<_binding>
{
  public:
    static constexpr GLenum TARGET = _target;

    Texture() { glGenTextures(1, &m_handle); }
    ~Texture() { glDeleteTextures(1, &m_handle); }

    /// @warning Deleted to prevent double deletion, use `std::unique_ptr` instead
    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other) = delete;

    void Bind() const { glBindTexture(TARGET, m_handle); }
    void Bind(GLenum unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(TARGET, m_handle);
    }
    void Unbind() const { glBindTexture(TARGET, 0); }

    /// @brief Gets active texture unit
    static GLint GetActiveUnit()
    {
        GLint unit;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &unit);
        return unit;
    }

    /**
     * @brief Set a texture parameter
     * @see glTexParameter
     *
     * @param pname The parameter to set
     * @param param The value of the parameter
     *
     * @note This function binds the texture
     */
    void Parameter(GLenum pname, GLint param)
    {
        Bind();
        glTexParameteri(TARGET, pname, param);
    }
    void Parameter(GLenum pname, GLfloat param)
    {
        Bind();
        glTexParameterf(TARGET, pname, param);
    }
    void Parameter(GLenum pname, const GLint* params)
    {
        Bind();
        glTexParameteriv(TARGET, pname, params);
    }
    void Parameter(GLenum pname, const GLfloat* params)
    {
        Bind();
        glTexParameterfv(TARGET, pname, params);
    }

    /**
     * @brief Generate a mipmap for this texture
     * @see glGenerateTextureMipmap
     */
    void GenerateMipmap()
    {
        glGenerateTextureMipmap(m_handle);
    }
};

class Texture1D : public Texture<GL_TEXTURE_1D, GL_TEXTURE_BINDING_1D>
{
  public:
    /**
     * @brief Set the texture image
     * @see glTexImage1D
     *
     * @param level The level-of-detail number
     * @param internalFormat The number of color components in the texture
     * @param width The width of the texture image
     * @param format The format of the pixel data
     * @param type The data type of the pixel data
     * @param data The image data
     *
     * @note This function binds the buffer
     */
    void Image(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
    {
        Bind();
        glTexImage1D(TARGET, level, internalFormat, width, 0, format, type, data);
    }
};

class Texture2D : public Texture<GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D>
{
  public:
    /**
     * @brief Set the texture image
     * @see glTexImage2D
     *
     * @param level The level-of-detail number
     * @param internalFormat The number of color components in the texture
     * @param width The width of the texture image
     * @param height The height of the texture image
     * @param format The format of the pixel data
     * @param type The data type of the pixel data
     * @param data The image data
     *
     * @note This function binds the buffer
     */
    void Image(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
    {
        Bind();
        glTexImage2D(TARGET, level, internalFormat, width, height, 0, format, type, data);
    }
};

class Texture3D : public Texture<GL_TEXTURE_3D, GL_TEXTURE_BINDING_3D>
{
  public:
    /**
     * @brief Set the texture image
     * @see glTexImage3D
     *
     * @param level The level-of-detail number
     * @param internalFormat The number of color components in the texture
     * @param width The width of the texture image
     * @param height The height of the texture image
     * @param depth The depth of the texture image
     * @param format The format of the pixel data
     * @param type The data type of the pixel data
     * @param data The image data
     *
     * @note This function binds the buffer
     */
    void Image(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* data)
    {
        Bind();
        glTexImage3D(TARGET, level, internalFormat, width, height, depth, 0, format, type, data);
    }
};

using Texture1DArray = Texture<GL_TEXTURE_1D_ARRAY, GL_TEXTURE_BINDING_1D_ARRAY>;
using Texture2DArray = Texture<GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BINDING_2D_ARRAY>;
using TextureCubeMap = Texture<GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BINDING_CUBE_MAP>;

} // namespace glwrap
