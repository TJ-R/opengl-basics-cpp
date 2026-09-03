#include <glad/glad.h>

#include "./headers/camera.h"
#include "./headers/model.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                              "OpenGLBasics-CPP", NULL, NULL);
        if (window == NULL) {
                std::cout << "Failed to create GLFW window.\n";
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cout << "Failed to initialize GLAD\n";
        }

        glEnable(GL_DEPTH_TEST);

        Shader objShader("./shaders/backpack.vert", "./shaders/backpack.frag");

        while (!glfwWindowShouldClose(window)) {
                float currentFrame = static_cast<float>(glfwGetTime());
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                processInput(window);

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                objShader.use();

                glm::mat4 projection = glm::perspective(
                    glm::radians(camera.Zoom),
                    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                glm::mat4 view = camera.GetViewMatrix();
                objShader.setMat4f("projection", projection);
                objShader.setMat4f("view", view);

                // render the loaded model
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(
                    model,
                    glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's
                                                  // at the center of the scene
                model = glm::scale(
                    model,
                    glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our
                                                  // scene, so scale it down
                objShader.setMat4f("model", model);

                std::string modelPath = "./models/backpack/backpack.obj";
                Model *backpack = new Model(modelPath.data());
                backpack->Draw(objShader);

                // glfw: swap buffers and poll IO events (keys
                // pressed/released, mouse moved etc.)
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
};

void processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse) {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const *path) {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data =
            stbi_load(path, &width, &height, &nrComponents, 0);

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
