#ifndef MESH_H
#define MESH_H

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
        std::string path;
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
        void Draw(Shader &shader) {
                // Bind Textures
                unsigned int diffuseNr = 1;
                unsigned int specularNr = 1;

                // This is a way to bind an unknown amount of textures
                // to the shader just need to make sure the max amount
                // textures is available in the shader
                for (int i = 0; i < textures.size(); i++) {
                        // Just increment the base enum by i
                        // should move to which enum it is using
                        glActiveTexture(GL_TEXTURE0 + i);

                        std::string name = textures[i].type;
                        std::string number;
                        if (name == "texture_diffuse") {
                                // Assuming that diffuseNr will increment in
                                // post
                                // ++diffuseNr would pre increment. I.e. one
                                // before to_string and one after
                                number = std::to_string(diffuseNr++);
                        } else if (name == "texture_specular") {
                                number = std::to_string(specularNr++);
                        }

                        // Store the texutres in some struct + name + number
                        // resulting in looking for material.texture_diffuse1
                        // or material.texutre_specular2, etc
                        shader.setInt("material." + name + number, i);
                        glBindTexture(GL_TEXTURE_2D, textures[i].id);
                }

                // Draw
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,
                               0);
                glBindVertexArray(0);
        }

      private:
        unsigned int VAO, VBO, EBO;
        void setupMesh() {
                // Need to make a combined vector of vertices and textures?
                // std::vector<float> data =

                glGenBuffers(1, &VBO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verticies.size(),
                             &verticies[0], GL_STATIC_DRAW);

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
#endif
