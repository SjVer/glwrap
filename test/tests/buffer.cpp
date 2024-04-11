#include <gtest/gtest.h>
#include <glwrap/buffer.hpp>

using namespace glwrap;

#define SUITE Buffer

TEST(SUITE, Create)
{
    ArrayBuffer vbo;

    EXPECT_EQ(vbo.BINDING, GL_ARRAY_BUFFER_BINDING);
    EXPECT_EQ(vbo.TARGET, GL_ARRAY_BUFFER);
    EXPECT_NE(vbo.Handle(), 0);
}

TEST(SUITE, Bind)
{
    ArrayBuffer vbo;

    EXPECT_EQ(ArrayBuffer::GetBound(), 0);
    vbo.Bind();
    EXPECT_EQ(ArrayBuffer::GetBound(), vbo.Handle());
    vbo.Unbind();
    EXPECT_EQ(ArrayBuffer::GetBound(), 0);
}

TEST(SUITE, Size)
{
    ArrayBuffer vbo;

    vbo.Initialize(1024, GL_STATIC_DRAW);
    EXPECT_EQ(vbo.Size(), 1024);

    vbo.Initialize(2048, GL_STATIC_DRAW);
    EXPECT_EQ(vbo.Size(), 2048);
}

TEST(SUITE, Data)
{
    ArrayBuffer vbo;

    float data[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    vbo.Store(4 * sizeof(float), GL_STATIC_DRAW, data);

    float* stored = (float*)vbo.Get();
    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(data[i], stored[i]);
    delete stored;

    float subData[2] = {5.0f, 6.0f};
    vbo.Write(1 * sizeof(float), subData, 2 * sizeof(float));

    float* subStored = (float*)vbo.Get(1 * sizeof(float), 2 * sizeof(float));
    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(subData[i], subStored[i]);

    delete subStored;
}