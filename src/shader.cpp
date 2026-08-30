#include "headers/shader.h"

Shader::Shader(const char *vShaderPath, const char *fShaderPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
                vShaderFile.open(vShaderPath);
                fShaderFile.open(fShaderPath);
                std::stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
        }

        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        // compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                          << infoLog << "\n";
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                          << infoLog << "\n";
        }

        programId = glCreateProgram();
        glAttachShader(programId, vertex);
        glAttachShader(programId, fragment);
        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
                glGetProgramInfoLog(programId, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                          << infoLog << "\n";
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
}

void Shader::use() {
        glUseProgram(programId);
}

void Shader::setBool(const std::string &name, bool val) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform1i(location, (int)val);
}

void Shader::setInt(const std::string &name, GLint val) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform1i(location, val);
}

void Shader::setFloat(const std::string &name, GLfloat val) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform1f(location, val);
}

void Shader::setVec3f(const std::string &name, glm::vec3 val) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform3f(location, val.x, val.y, val.z);
}

void Shader::setVec3f(const std::string &name, GLfloat x, GLfloat y,
                      GLfloat z) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform3f(location, x, y, z);
}

void Shader::setVec4f(const std::string &name, glm::vec4 val) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform4f(location, val.x, val.y, val.z, val.w);
}

void Shader::setVec4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z,
                      GLfloat w) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform4f(location, x, y, z, w);
}

void Shader::setMat4f(const std::string &name, glm::mat4 val) {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
}
