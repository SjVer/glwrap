cmake_minimum_required(VERSION 3.5)

project(glwrap_tests)

include_directories(${CMAKE_SOURCE_DIR}/..)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(glad include/glad/glad.h glad.c)
link_directories(${CMAKE_SOURCE_DIR}/lib)
add_link_options(/NODEFAULTLIB:MSVCRT)

file(GLOB TestSources tests/*.cpp)
add_executable(tests main.cpp ${TestSources})
target_link_libraries(tests glad GTest::gtest glfw3)

include(GoogleTest)
gtest_discover_tests(tests)