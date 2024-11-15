#pragma once

#include "mySceneExp.hpp"

class Shader {
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
    
        void use() {glUseProgram(program);}

        void setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const{
            glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
        }
        void setUniformVec3(const std::string &name, const glm::vec3 &vector) const{
            glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &vector[0]);
        }

        void setUniformVec4(const std::string &name, const glm::vec4 &vector) const{
            glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &vector[0]);
        }

        void setUniformInt(const std::string &name, int value) const{
            glUniform1i(glGetUniformLocation(program, name.c_str()), value);
        }

        void setUniformBool(const std::string &name, bool value) const{
            glUniform1i(glGetUniformLocation(program, name.c_str()), value);
        }

        ~Shader();

    private:
        static const unsigned int NUM_SHADERS = 2;

        GLuint program;
        GLuint shaders[NUM_SHADERS];

        static std::string loadShader(const std::string& fileName);

        static GLuint createShader(const std::string &text, GLenum shaderType);

        static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
};