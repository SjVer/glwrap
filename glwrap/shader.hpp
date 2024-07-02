#pragma once

#include <istream>
#include <string>
#include <vector>

#include "glwrap/include_gl.h"
#include "glwrap/object.hpp"

namespace glwrap
{

/**
 * @brief A single shader
 */
template <GLenum _type>
class Shader
{
  protected:
    GLuint m_handle;

  public:
    static inline GLenum TYPE = _type;

    Shader() { m_handle = glCreateShader(TYPE); }
    ~Shader() { glDeleteShader(m_handle); }

    /// @warning Deleted to prevent double deletion, use `std::unique_ptr` instead
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other) = delete;

    inline GLuint Handle() const { return m_handle; }

    /**
     * @brief Sets the shader source
     * @see glShaderSource
     */
    void Source(const char* source)
    {
        glShaderSource(m_handle, 1, &source, nullptr);
    }

    static Shader<_type> FromSource(const char* source)
    {
        Shader<_type> shader;
        shader.Source(source);
        return shader;
    }

    /**
     * @brief Sets the shader source from a file
     * @see glShaderSource
     */
    void SourceFile(const char* path)
    {
        using namespace std;

        ifstream s(path);
        string str((istreambuf_iterator<char>(s)), istreambuf_iterator<char>());
        s.close();

        Source(str.c_str());
    }

    static Shader<_type> FromSourceFile(const char* path)
    {
        Shader<_type> shader;
        shader.SourceFile(path);
        return shader;
    }

    /**
     * @brief Compiles the shader
     * @see glCompileShader
     *
     * @return Whether the shader compiled successfully
     */
    bool Compile()
    {
        glCompileShader(m_handle);
        return GetCompileStatus();
    }

    /// @see glGetShaderiv
    bool GetCompileStatus() const
    {
        GLint status;
        glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
        return status == GL_TRUE;
    }

    /**
     * @brief Gets the shader's info log
     * @see glGetShaderInfoLog
     */
    std::string GetInfoLog() const
    {
        GLint length;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, '\0');
        glGetShaderInfoLog(m_handle, length, nullptr, log.data());

        return log;
    }
};

using VertexShader = Shader<GL_VERTEX_SHADER>;
using FragmentShader = Shader<GL_FRAGMENT_SHADER>;

/**
 * @brief A shader program
 */
class Program : public Object<GL_CURRENT_PROGRAM>
{
  public:
    Program() { m_handle = glCreateProgram(); }
    ~Program() { glDeleteProgram(m_handle); }

    Program(const Program& other) = delete;
    Program& operator=(const Program& other) = delete;
    Program(Program&& other) = delete;

    void Use() const { glUseProgram(m_handle); }
    void Unuse() const { glUseProgram(0); }

    /**
     * @brief Attaches a shader to the program
     * @see glAttachShader
     */
    template <GLenum type>
    void Attach(const Shader<type>& shader)
    {
        glAttachShader(m_handle, shader.Handle());
    }

    /*
     * @brief Unattaches a shader from the program
     * @see glDetachShader
     */
    template <GLenum type>
    void Detach(const Shader<type>& shader)
    {
        glDetachShader(m_handle, shader.Handle());
    }

    /**
     * @brief Links the program
     * @see glLinkProgram
     *
     * @return Whether the program linked successfully
     */
    bool Link()
    {
        glLinkProgram(m_handle);
        glValidateProgram(m_handle);
        return GetLinkStatus();
    }

    /// @see glGetProgramiv
    bool GetLinkStatus() const
    {
        GLint status;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
        return status == GL_TRUE;
    }

    /**
     * @brief Gets the program's info log
     * @see glGetProgramInfoLog
     */
    std::string GetInfoLog() const
    {
        GLint length;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, '\0');
        glGetProgramInfoLog(m_handle, length, nullptr, log.data());

        return log;
    }

    /**
     * @brief Gets the number of active uniforms
     * @see glGetProgramiv
     */
    GLint GetUniformCount() const
    {
        GLint count;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count);
        return count;
    }

    /**
     * @brief Gets the location of a uniform variable
     * @see glGetUniformLocation
     */
    GLint GetUniformLocation(const char* name) const
    {
        return glGetUniformLocation(m_handle, name);
    }
};

/**
 * @brief A shader program with uniform management
 */
class ShaderManager : public Program
{
  protected:
    std::vector<std::string> m_uniforms = {};

  public:
    ShaderManager() = default;
    ~ShaderManager() = default;

    ShaderManager(const ShaderManager& other) = delete;
    ShaderManager& operator=(const ShaderManager& other) = delete;
    ShaderManager(ShaderManager&& other) = delete;

    /**
     * @brief Links the program
     * @see glLinkProgram
     *
     * @return Whether the program linked successfully
     */
    bool Link()
    {
        m_uniforms.clear();

        if (!Program::Link()) return false;

        // gather uniforms

        GLint count, maxLength;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count);
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

        int length;
        GLint size;
        GLenum type;
        GLchar* name = new GLchar[maxLength];
        for (int i = 0; i < count; i++)
        {
            glGetActiveUniform(
                m_handle, i, maxLength, &length,
                &size, &type, name
            );
            m_uniforms.push_back(std::string(name, length));
        }

        return true;
    }

    /**
     * @brief Gets the number of active uniforms
     * @see glGetProgramiv
     */
    GLint GetUniformCount() const
    {
        return static_cast<GLint>(m_uniforms.size());
    }

    /**
     * @brief Gets the location of a uniform variable
     * @see glGetUniformLocation
     */
    GLint GetUniformLocation(const char* name) const
    {
        auto it = std::find(m_uniforms.begin(), m_uniforms.end(), name);
        if (it == m_uniforms.end()) return -1;
        return static_cast<GLint>(it - m_uniforms.begin());
    }
};

} // namespace glwrap