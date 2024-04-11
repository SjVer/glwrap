#include <glad/glad.h>
#include <memory>

#include "glwrap/resource.hpp"

namespace glwrap
{

/**
 * @brief A buffer object
 *
 * @tparam target The target for `glBindBuffer`
 * @tparam binding The binding for `glGet`
 */
template <GLenum target, GLenum binding>
class Buffer : public Resource<binding>
{
  protected:
    void BindIfUnbound() const
    {
        if(GetBound() != m_handle)
            Bind();
    }

  public:
    static inline GLenum TARGET = target;

    Buffer() { glGenBuffers(1, &m_handle); }
    ~Buffer() { glDeleteBuffers(1, &m_handle); }

    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;
    Buffer(Buffer&& other) = delete;

    void Bind() const { glBindBuffer(target, m_handle); }
    void Unbind() const { glBindBuffer(target, 0); }

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
        glBufferData(target, size, data, usage);
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
        glBufferSubData(target, offset, size, data);
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
        glGetBufferSubData(target, offset, size, data);
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
        return glMapBuffer(target, access);
    }

    /**
     * @brief Unmaps the buffer's data store
     * @see glUnmapBuffer
     */
    void Unmap()
    {
        BindIfUnbound();
        glUnmapBuffer(target);
    }

    /**
     * @brief Returns the size of the buffer's data store in bytes
     * @see glGetBufferParameteriv
     */
    GLsizeiptr Size() const
    {
        BindIfUnbound();
        GLint size;
        glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
        return size;
    }
};

/// @brief A buffer with target `GL_ARRAY_BUFFER` and binding `GL_ARRAY_BUFFER_BINDING`
using ArrayBuffer = Buffer<GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING>;

/// @brief A buffer with target `GL_COPY_READ_BUFFER` and binding `GL_COPY_READ_BUFFER_BINDING`
using CopyReadBuffer = Buffer<GL_COPY_READ_BUFFER, GL_COPY_READ_BUFFER_BINDING>;

/// @brief A buffer with target `GL_COPY_WRITE_BUFFER` and binding `GL_COPY_WRITE_BUFFER_BINDING`
using CopyWriteBuffer = Buffer<GL_COPY_WRITE_BUFFER, GL_COPY_WRITE_BUFFER_BINDING>;

/// @brief A buffer with target `GL_ELEMENT_ARRAY_BUFFER` and binding `GL_ELEMENT_ARRAY_BUFFER_BINDING`
using ElementArrayBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER_BINDING>;

/// @brief A buffer with target `GL_PIXEL_PACK_BUFFER` and binding `GL_PIXEL_PACK_BUFFER_BINDING`
using PixelPackBuffer = Buffer<GL_PIXEL_PACK_BUFFER, GL_PIXEL_PACK_BUFFER_BINDING>;

/// @brief A buffer with target `GL_PIXEL_UNPACK_BUFFER` and binding `GL_PIXEL_UNPACK_BUFFER_BINDING`
using PixelUnpackBuffer = Buffer<GL_PIXEL_UNPACK_BUFFER, GL_PIXEL_UNPACK_BUFFER_BINDING>;

/// @brief A buffer with target `GL_TRANSFORM_FEEDBACK_BUFFER` and binding `GL_TRANSFORM_FEEDBACK_BUFFER_BINDING`
using TransformFeedbackBuffer = Buffer<GL_TRANSFORM_FEEDBACK_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER_BINDING>;

/// @brief A buffer with target `GL_UNIFORM_BUFFER` and binding `GL_UNIFORM_BUFFER_BINDING`
using UniformBuffer = Buffer<GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING>;

/// @brief A buffer with target `GL_ATOMIC_COUNTER_BUFFER` and binding `GL_ATOMIC_COUNTER_BUFFER_BINDING`
using AtomicCounterBuffer = Buffer<GL_ATOMIC_COUNTER_BUFFER, GL_ATOMIC_COUNTER_BUFFER_BINDING>;

/// @brief A buffer with target `GL_DISPATCH_INDIRECT_BUFFER` and binding `GL_DISPATCH_INDIRECT_BUFFER_BINDING`
using DispatchIndirectBuffer = Buffer<GL_DISPATCH_INDIRECT_BUFFER, GL_DISPATCH_INDIRECT_BUFFER_BINDING>;

/// @brief A buffer with target `GL_DRAW_INDIRECT_BUFFER` and binding `GL_DRAW_INDIRECT_BUFFER_BINDING`
using DrawIndirectBuffer = Buffer<GL_DRAW_INDIRECT_BUFFER, GL_DRAW_INDIRECT_BUFFER_BINDING>;

/// @brief A buffer with target `GL_QUERY_BUFFER` and binding `GL_QUERY_BUFFER_BINDING`
using QueryBuffer = Buffer<GL_QUERY_BUFFER, GL_QUERY_BUFFER_BINDING>;

/// @brief A buffer with target `GL_SHADER_STORAGE_BUFFER` and binding `GL_SHADER_STORAGE_BUFFER_BINDING`
using ShaderStorageBuffer = Buffer<GL_SHADER_STORAGE_BUFFER, GL_SHADER_STORAGE_BUFFER_BINDING>;

/// @brief A buffer with target `GL_TEXTURE_BUFFER` and binding `GL_TEXTURE_BUFFER_BINDING`
using TextureBuffer = Buffer<GL_TEXTURE_BUFFER, GL_TEXTURE_BUFFER_BINDING>;

} // namespace glwrap