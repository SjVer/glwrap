#include <gtest/gtest.h>
#include <glwrap/vertex_array.hpp>

using namespace glwrap;

#define SUITE VertexArray

TEST(SUITE, Create)
{
    VertexArray vao;

    EXPECT_EQ(vao.BINDING, GL_VERTEX_ARRAY_BINDING);
    EXPECT_NE(vao.Handle(), 0);
}

TEST(SUITE, Bind)
{
    VertexArray vao;

    EXPECT_EQ(VertexArray::GetBound(), 0);
    vao.Bind();
    EXPECT_EQ(VertexArray::GetBound(), vao.Handle());
    vao.Unbind();
    EXPECT_EQ(VertexArray::GetBound(), 0);
}