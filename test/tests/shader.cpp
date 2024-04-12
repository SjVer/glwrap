#include <gtest/gtest.h>
#include <glwrap/shader.hpp>

using namespace glwrap;

#define SUITE Shader

TEST(SUITE, CreateShader)
{
    VertexShader shader;
    EXPECT_EQ(shader.TYPE, GL_VERTEX_SHADER);
    EXPECT_NE(shader.Handle(), 0);

    FragmentShader fragmentShader;
    EXPECT_EQ(fragmentShader.TYPE, GL_FRAGMENT_SHADER);
    EXPECT_NE(fragmentShader.Handle(), 0);
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
    Program program;
    EXPECT_NE(program.Handle(), 0);
}

TEST(SUITE, LinkProgram)
{
    Program program;
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
    Program program;
    VertexShader vertexShader;
    FragmentShader fragmentShader;

    vertexShader.Source(
        "#version 330 core\n"
        "void main() { }"
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

    EXPECT_EQ(program.GetUniformCount(), 1);
    EXPECT_NE(program.GetUniformLocation("color"), -1);
}

TEST(SUITE, Manager)
{
    ShaderManager manager;
    VertexShader vertexShader;
    FragmentShader fragmentShader;

    vertexShader.Source(
        "#version 330 core\n"
        "void main() { }"
    );

    fragmentShader.Source(
        "#version 330 core\n"
        "uniform vec4 color;\n"
        "out vec4 fragColor;\n"
        "void main() { fragColor = color; }"
    );

    EXPECT_TRUE(vertexShader.Compile());
    EXPECT_TRUE(fragmentShader.Compile());

    manager.Attach(vertexShader);
    manager.Attach(fragmentShader);

    EXPECT_TRUE(manager.Link());
    EXPECT_TRUE(manager.GetLinkStatus());

    EXPECT_EQ(manager.GetUniformCount(), manager.Program::GetUniformCount());

    EXPECT_NE(manager.GetUniformLocation("color"), -1);
    EXPECT_EQ(
        manager.GetUniformLocation("color"),
        manager.Program::GetUniformLocation("color")
    );
}