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
        void setInt(std::string name, int val);
        void setFloat(std::string name, float val);
        void setVec3f(std::string name, glm::vec3 val);
        void setVec3f(std::string name, float x, float y, float z);
        void setVec4f(std::string name, glm::vec4 val);
        void setVec4f(std::string name, float x, float y, float z, float w);
        void setMat4f(std::string name, glm::mat4 mat);

        void compileShader(const char *src, std::uint32_t shaderType);

        // private:
};
