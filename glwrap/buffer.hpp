#pragma once

#include <memory>

#include "glwrap/include_gl.h"
#include "glwrap/object.hpp"

namespace glwrap
{

/**
 * @brief A buffer object
 *
 * @tparam target The target for `glBindBuffer`
 * @tparam binding The binding for `glGet`
 */
template <GLenum _target, GLenum _binding>
class Buffer : public Object<_binding>
{
  protected:
    inline void BindIfUnbound() const
    {
        if (!IsBound()) Bind();
    }

  public:
    static inline GLenum TARGET = _target;

    Buffer() { glGenBuffers(1, &m_handle); }
    ~Buffer() { glDeleteBuffers(1, &m_handle); }

    /// @warning Deleted to prevent double deletion, use `std::unique_ptr` instead
    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;
    Buffer(Buffer&& other) = delete;

    void Bind() const { glBindBuffer(TARGET, m_handle); }
    void Unbind() const { glBindBuffer(TARGET, 0); }

    /**
     * @brief Creates and writes to the buffer's data storage
     * @see glBufferData
     *
     * @param size The size in bytes of the buffer
     * @param usage The expected usage of the data
     * @param data The initial data or `nullptr` to leave uninitialized
     */
    void Store(GLsizeiptr size, GLenum usage, const void* data)
    {
        BindIfUnbound();
        glBufferData(TARGET, size, data, usage);
    }

    /// @brief An alias for `Store(size, usage, nullptr)`
    inline void Initialize(GLsizeiptr size, GLenum usage)
    {
        Store(size, usage, nullptr);
    }

    /**
     * @brief Replaces a subset of the buffer's data store
     * @see glBufferSubData
     *
     * @param offset The offset into the buffer object's data, in bytes
     * @param data The new data to be copied into the data store
     * @param size The size in bytes of the data being overwritten
     */
    void Write(GLintptr offset, const void* data, GLsizeiptr size)
    {
        BindIfUnbound();
        glBufferSubData(TARGET, offset, size, data);
    }

    /**
     * @brief Gets a subset of the buffer's data store
     * @see glGetBufferSubData
     *
     * @param offset The offset into the buffer object's data, in bytes
     * @param size The size in bytes of the data being retrieved
     * @return A pointer to the data, allocated with `new`
     */
    void* Get(GLintptr offset, GLsizeiptr size)
    {
        BindIfUnbound();
        void* data = new char[size];
        glGetBufferSubData(TARGET, offset, size, data);
        return data;
    }

    /**
     * @brief An alias for `Get(0, Size())`
     * @return A pointer to the data, allocated with `new`
     */
    inline void* Get() { return Get(0, Size()); }

    /**
     * @brief Maps the buffer's data store into the client's address space
     * @see glMapBuffer
     *
     * @param access The access policy
     * @return A pointer to the data
     */
    void* Map(GLenum access)
    {
        BindIfUnbound();
        return glMapBuffer(TARGET, access);
    }

    /**
     * @brief Unmaps the buffer's data store
     * @see glUnmapBuffer
     */
    void Unmap()
    {
        BindIfUnbound();
        glUnmapBuffer(TARGET);
    }

    /**
     * @brief Returns the size of the buffer's data store in bytes
     * @see glGetBufferParameteriv
     */
    GLsizeiptr Size() const
    {
        BindIfUnbound();
        GLint size;
        glGetBufferParameteriv(TARGET, GL_BUFFER_SIZE, &size);
        return size;
    }
};

/// @brief A buffer with target `GL_ARRAY_BUFFER` and binding `GL_ARRAY_BUFFER_BINDING`
using ArrayBuffer = Buffer<GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING>;

/// @brief A buffer with target `GL_ELEMENT_ARRAY_BUFFER` and binding `GL_ELEMENT_ARRAY_BUFFER_BINDING`
using ElementArrayBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER_BINDING>;

/// @brief A buffer with target `GL_PIXEL_PACK_BUFFER` and binding `GL_PIXEL_PACK_BUFFER_BINDING`
using PixelPackBuffer = Buffer<GL_PIXEL_PACK_BUFFER, GL_PIXEL_PACK_BUFFER_BINDING>;

/// @brief A buffer with target `GL_PIXEL_UNPACK_BUFFER` and binding `GL_PIXEL_UNPACK_BUFFER_BINDING`
using PixelUnpackBuffer = Buffer<GL_PIXEL_UNPACK_BUFFER, GL_PIXEL_UNPACK_BUFFER_BINDING>;

/// @brief A buffer with target `GL_TRANSFORM_FEEDBACK_BUFFER` and binding `GL_TRANSFORM_FEEDBACK_BUFFER_BINDING`
using TransformFeedbackBuffer = Buffer<GL_TRANSFORM_FEEDBACK_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER_BINDING>;

} // namespace glwrap