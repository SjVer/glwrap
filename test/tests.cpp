#include <glwrap/buffer.hpp>

#include <gtest/gtest.h>
#include <GLFW/glfw3.h>

class GLTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        // setup glfw
        if(!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW.\n");
            exit(1);
        }

        // create context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
        if(!window)
        {
            const char* msg;
            glfwGetError(&msg);
            fprintf(stderr, "Failed to create window: %s\n", msg);
            exit(1);
        }
        glfwMakeContextCurrent(window);

        // initialize glad
        if(!gladLoadGL())
        {
            fprintf(stderr, "Failed to initialize glad.\n");
            exit(1);
        }

        printf("Using OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    }
};

GTEST_TEST_F(GLTest, Create)
{
    glwrap::ArrayBuffer vao;
    EXPECT_NE(vao.Handle(), 0);
}

GTEST_TEST_F(GLTest, Bind)
{
    glwrap::ArrayBuffer vao;

    EXPECT_EQ(glwrap::ArrayBuffer::GetBound(), 0);
    vao.Bind();
    EXPECT_EQ(glwrap::ArrayBuffer::GetBound(), vao.Handle());
    vao.Unbind();
    EXPECT_EQ(glwrap::ArrayBuffer::GetBound(), 0);
}

GTEST_TEST_F(GLTest, Size)
{
    glwrap::ArrayBuffer vao;

    vao.Initialize(1024, GL_STATIC_DRAW);
    EXPECT_EQ(vao.Size(), 1024);

    vao.Initialize(2048, GL_STATIC_DRAW);
    EXPECT_EQ(vao.Size(), 2048);
}

GTEST_TEST_F(GLTest, Data)
{
    glwrap::ArrayBuffer vao;

    float data[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    vao.Store(4 * sizeof(float), GL_STATIC_DRAW, data);

    float* stored = (float*)vao.Get();
    for(int i = 0; i < 4; ++i)
        EXPECT_EQ(data[i], stored[i]);
    delete stored;

    float subData[2] = {5.0f, 6.0f};
    vao.Write(1 * sizeof(float), subData, 2 * sizeof(float));

    float* subStored = (float*)vao.Get(1 * sizeof(float), 2 * sizeof(float));
    for(int i = 0; i < 2; ++i)
        EXPECT_EQ(subData[i], subStored[i]);

    delete subStored;
}