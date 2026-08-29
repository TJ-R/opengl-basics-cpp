cmake_minimum_required(VERSION 3.23)
project(OpenGLBasics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_library(glad STATIC external/glad/src/glad.c)
add_library(stb_image INTERFACE)

target_sources(stb_image INTERFACE
    FILE_SET HEADERS 
    BASE_DIRS external
    FILES external/stb_image/stb_image.h
)

add_subdirectory(external/glfw-3.5.1)
add_subdirectory(external/glm)

target_include_directories(glad PUBLIC external/glad/include)

find_package(OpenGL REQUIRED)

add_executable(OpenGLBasics src/main.cpp)

target_sources(OpenGLBasics PRIVATE
    src/shader.cpp
    src/headers/shader.h
    src/headers/camera.h
)

target_link_libraries(OpenGLBasics PRIVATE
    glfw
    glm
    OpenGL::GL
    glad
    stb_image
)
