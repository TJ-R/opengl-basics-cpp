#include <glad/glad.h>

#include "shader.h"
#include <glm/glm.hpp>
#include <string>

struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TextureCoords;
};

struct Texture {
        unsigned int id;
        std::string type;
};

class Mesh {
      public:
        std::vector<Vertex> verticies;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indicies,
             std::vector<Texture> textures) {
                this->verticies = verticies;
                this->indices = indicies;
                this->textures = textures;

                setupMesh();
        }
        void draw(Shader &shader);

      private:
        unsigned int VAO, VBO, EBO;
        void setupMesh() {
                // Need to make a combined vector of vertices and textures?
                // std::vector<float> data =

                void *data = static_cast<void *>(this->verticies.data());

                glGenBuffers(1, &VBO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticies.size(),
                             data, GL_STATIC_DRAW);
        };
};
