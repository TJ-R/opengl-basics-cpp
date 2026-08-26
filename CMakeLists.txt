cmake_minimum_required(VERSION 3.13)
project(OpenGLBasics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_library(glad STATIC external/glad/src/glad.c)

add_subdirectory(external/glfw-3.5.1)
add_subdirectory(external/glm)

target_include_directories(glad PUBLIC external/glad/include)

find_package(OpenGL REQUIRED)

add_executable(OpenGLBasics src/main.cpp)
target_link_libraries(OpenGLBasics PRIVATE
    glfw
    glm
    OpenGL::GL
    glad
)
