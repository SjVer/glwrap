#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

static void SetupGL()
{
    // setup glfw
    if (!glfwInit())
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
    if (!window)
    {
        const char* msg;
        glfwGetError(&msg);
        fprintf(stderr, "Failed to create window: %s\n", msg);
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // initialize glad
    if (!gladLoadGL())
    {
        fprintf(stderr, "Failed to initialize glad.\n");
        exit(1);
    }
}

int main(int argc, char** argv)
{
    SetupGL();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}