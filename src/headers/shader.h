// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

class Shader {
      public:
        Shader(std::string path);
        ~Shader();

        std::string id;

        void setBool(std::string name, bool val);
        void setInt(std::string name, GLint val);
        void setFloat(std::string name, GLfloat val);
        void setVec3f(std::string name, glm::vec3 val);
        void setVec3f(std::string name, GLfloat x, GLfloat y, GLfloat z);
        void setVec4f(std::string name, glm::vec4 val);
        void setVec4f(std::string name, GLfloat x, GLfloat y, GLfloat z,
                      GLfloat w);
        void setMat4f(std::string name, glm::mat4 mat);

        void use();

      private:
        void compile(const char *src, GLuint shaderType);
};
