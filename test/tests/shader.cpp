#include <gtest/gtest.h>
#include <glwrap/shader.hpp>

using namespace glwrap;

#define SUITE Shader

TEST(SUITE, CreateShader)
{
    Shader<GL_VERTEX_SHADER> shader;
    EXPECT_EQ(shader.TYPE, GL_VERTEX_SHADER);
    EXPECT_NE(shader.Handle(), 0);
}

TEST(SUITE, CompileShader)
{
    Shader<GL_VERTEX_SHADER> shader;
    shader.Source(
        "#version 330 core\n"
        "void main() { gl_Position = vec4(0.0); }"
    );

    EXPECT_TRUE(shader.Compile());
    EXPECT_TRUE(shader.GetCompileStatus());
}

TEST(SUITE, CompileShaderError)
{
    Shader<GL_VERTEX_SHADER> shader;
    shader.Source(
        "#version 330 core\n"
        "void main() { gl_Position = vec4(0.0) }"
    );

    EXPECT_FALSE(shader.Compile());
    EXPECT_FALSE(shader.GetCompileStatus());
}

TEST(SUITE, CreateProgram)
{
    ShaderProgram program;
    EXPECT_NE(program.Handle(), 0);
}

TEST(SUITE, LinkProgram)
{
    ShaderProgram program;
    Shader<GL_VERTEX_SHADER> vertexShader;
    Shader<GL_FRAGMENT_SHADER> fragmentShader;

    vertexShader.Source(
        "#version 330 core\n"
        "void main() { gl_Position = vec4(0.0); }"
    );

    fragmentShader.Source(
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() { color = vec4(1.0); }"
    );

    EXPECT_TRUE(vertexShader.Compile());
    EXPECT_TRUE(fragmentShader.Compile());

    program.Attach(vertexShader);
    program.Attach(fragmentShader);

    EXPECT_TRUE(program.Link());
    EXPECT_TRUE(program.GetLinkStatus());
}

TEST(SUITE, Uniforms)
{
    ShaderProgram program;
    Shader<GL_VERTEX_SHADER> vertexShader;
    Shader<GL_FRAGMENT_SHADER> fragmentShader;

    vertexShader.Source(
        "#version 330 core\n"
        "uniform mat4 model;\n"
        "void main() { gl_Position = model * vec4(0.0); }"
    );

    fragmentShader.Source(
        "#version 330 core\n"
        "uniform vec4 color;\n"
        "out vec4 fragColor;\n"
        "void main() { fragColor = color; }"
    );

    EXPECT_TRUE(vertexShader.Compile());
    EXPECT_TRUE(fragmentShader.Compile());

    program.Attach(vertexShader);
    program.Attach(fragmentShader);

    EXPECT_TRUE(program.Link());
    EXPECT_TRUE(program.GetLinkStatus());

    EXPECT_EQ(
        glGetUniformLocation(program.Handle(), "model"),
        program.GetUniformLocation("model")
    );
    EXPECT_EQ(
        glGetUniformLocation(program.Handle(), "color"),
        program.GetUniformLocation("color")
    );
}