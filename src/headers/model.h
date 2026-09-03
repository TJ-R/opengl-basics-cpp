#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image/stb_image.h>

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma = false);
class Model {
      public:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
        Model(char *path) {
                loadModel(path);
        }
        void Draw(Shader &shader) {
                for (int i = 0; i < meshes.size(); i++) {
                        meshes[i].Draw(shader);
                }
        };

      private:
        void loadModel(std::string path) {
                Assimp::Importer import;
                const aiScene *scene = import.ReadFile(
                    path, aiProcess_Triangulate | aiProcess_FlipUVs);

                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
                    !scene->mRootNode) {
                        std::cout
                            << "ERROR::ASSIMP::" << import.GetErrorString()
                            << "\n";
                }
                // This might need to be \ for linux
                directory = path.substr(0, path.find_last_of('/'));
                processNode(scene->mRootNode, scene);
        };
        void processNode(aiNode *node, const aiScene *scene) {
                // Go into each node and for each mesh in the node
                // and process the mesh then add the proccessed mesh to the
                // vector of meshes
                for (int i = 0; i < node->mNumMeshes; i++) {
                        aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
                        meshes.push_back(processMesh(aiMesh, scene));
                }

                for (int i = 0; i < node->mNumChildren; i++) {
                        processNode(node->mChildren[i], scene);
                }
        };
        Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
                std::vector<Vertex> vertices;
                std::vector<unsigned int> indicies;
                std::vector<Texture> textures;

                glm::vec3 vector;
                // Build Verticies
                for (int i = 0; i < mesh->mNumVertices; i++) {
                        Vertex vertex;
                        vector.x = mesh->mVertices[i].x;
                        vector.y = mesh->mVertices[i].y;
                        vector.z = mesh->mVertices[i].z;
                        vertex.Position = vector;

                        if (mesh->HasNormals()) {
                                vector.x = mesh->mNormals[i].x;
                                vector.y = mesh->mNormals[i].y;
                                vector.z = mesh->mNormals[i].z;
                                vertex.Normal = vector;
                        }

                        // Somthing feels off about this one
                        if (mesh->mTextureCoords[0]) {
                                glm::vec2 vector;
                                vector.x = mesh->mTextureCoords[0][i].x;
                                vector.y = mesh->mTextureCoords[0][i].y;
                                vertex.TextureCoords = vector;
                        } else {
                                vertex.TextureCoords = glm::vec2(0.0f, 0.0f);
                                std::cout << "No Tex Coords\n";
                        }

                        vertices.push_back(vertex);
                }
                // Build Indicies
                for (int i = 0; i < mesh->mNumFaces; i++) {
                        aiFace face = mesh->mFaces[i];
                        for (int j = 0; j < face.mNumIndices; j++) {
                                indicies.push_back(face.mIndices[j]);
                        }
                }

                // Build Textures
                if (mesh->mMaterialIndex >= 0) {
                        aiMaterial *material =
                            scene->mMaterials[mesh->mMaterialIndex];

                        std::vector<Texture> diffuseMaps = loadMaterialTexture(
                            material, aiTextureType_DIFFUSE, "texture_diffuse");
                        textures.insert(textures.end(), diffuseMaps.begin(),
                                        diffuseMaps.end());

                        std::vector<Texture> specularMaps = loadMaterialTexture(
                            material, aiTextureType_SPECULAR,
                            "texture_specular");
                        textures.insert(textures.end(), specularMaps.begin(),
                                        specularMaps.end());
                }

                return Mesh(vertices, indicies, textures);
        }
        std::vector<Texture> loadMaterialTexture(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName) {
                std::vector<Texture> textures;

                for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
                        aiString str;
                        mat->GetTexture(type, i, &str);

                        bool skip = false;
                        for (unsigned int j = 0; j < textures_loaded.size();
                             j++) {
                                if (std::strcmp(textures_loaded[j].path.data(),
                                                str.C_Str()) == 0) {
                                        textures.push_back(textures_loaded[j]);
                                        skip = true;
                                        break;
                                }
                        }

                        if (!skip) {
                                Texture texture;
                                texture.id =
                                    TextureFromFile(str.C_Str(), directory);
                                texture.type = typeName;
                                texture.path = str.C_Str();
                                textures_loaded.push_back(texture);
                                textures.push_back(texture);
                        }
                }

                return textures;
        }
};

unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma) {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data =
            stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
                GLenum format;
                if (nrComponents == 1) {
                        format = GL_RED;
                } else if (nrComponents == 3) {
                        format = GL_RGB;
                } else if (nrComponents == 4) {
                        format = GL_RGBA;
                }

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                             GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);

                stbi_image_free(data);
        } else {
                std::cout << "Texture failed to load at path: " << path << "\n";
                stbi_image_free(data);
        }

        return textureID;
}
#endif
