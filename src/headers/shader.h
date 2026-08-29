#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
      public:
        Shader(const char *vertexShaderPath, const char *fragmentShaderPath);

        GLuint programId;

        void use();

        void setBool(const char *name, bool val);
        void setInt(const char *name, GLint val);
        void setFloat(const char *name, GLfloat val);
        void setVec3f(const char *name, glm::vec3 val);
        void setVec3f(const char *name, GLfloat x, GLfloat y, GLfloat z);
        void setVec4f(const char *name, glm::vec4 val);
        void setVec4f(const char *name, GLfloat x, GLfloat y, GLfloat z,
                      GLfloat w);
        void setMat4f(const char *name, glm::mat4 mat);
};
