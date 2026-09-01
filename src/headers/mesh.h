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

                glGenBuffers(1, &VBO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), &verticies[0],
                             GL_STATIC_DRAW);

                glGenVertexArrays(1, &VAO);
                glBindVertexArray(VAO);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                      8 * sizeof(float), 0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                      8 * sizeof(float),
                                      (void *)offsetof(Vertex, Normal));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                                      8 * sizeof(float),
                                      (void *)offsetof(Vertex, TextureCoords));
                glEnableVertexAttribArray(2);

                glGenBuffers(1, &EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             sizeof(unsigned int) * indices.size(), &indices[0],
                             GL_STATIC_DRAW);

                // Unbinding the array buffer when EBO had data so it can be
                // reused anytime the VAO is bound
                glBindVertexArray(0);
        };
};
