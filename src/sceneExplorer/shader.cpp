#include "shader.hpp"

using std::string;

Shader::Shader(const string &vertexPath, const string &fragmentPath){
    program = glCreateProgram();
    shaders[0] = createShader(loadShader(vertexPath), GL_VERTEX_SHADER);
    shaders[1] = createShader(loadShader(fragmentPath), GL_FRAGMENT_SHADER);

    for(int i = 0; i < NUM_SHADERS; i++){
        glAttachShader(program, shaders[i]);
    }

    glLinkProgram(program);
    checkShaderError(program, GL_LINK_STATUS, true, "Error: Program linking failed");

    glValidateProgram(program);
    checkShaderError(program, GL_VALIDATE_STATUS, true, "Error: Program is invalid");
}

string Shader::loadShader(const string& fileName){
    std::ifstream file;
    file.open((fileName).c_str());
    string output;
    string line;

    if(file.is_open()){
        while(file.good()) {
            getline(file, line);
			output.append(line + "\n");
        }
    } else {
		fatalError("Unable to load shader " + fileName);
    }
    return output;
}

GLuint Shader::createShader(const string& text, GLenum shaderType){ 
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) fatalError("Error: Shader creation failed");
    
    const GLchar *shaderSrcStrings[1];
    GLint shaderSrcStringsLengths[1];

    shaderSrcStrings[0] = text.c_str();
    shaderSrcStringsLengths[0] = text.length();

    glShaderSource(shader, 1, shaderSrcStrings, shaderSrcStringsLengths);
    glCompileShader(shader);

    checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed");

    return shader;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage){
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram){
        glGetProgramiv(shader, flag, &success);
    } else {
        glGetShaderiv(shader, flag, &success);
    }

    if(success == GL_FALSE) {
        if(isProgram){
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        } else {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }
        fatalError(errorMessage + ": '" + error + "'");
    }
}

Shader::~Shader() {
    for(int i = 0; i < NUM_SHADERS; i++){
        // detach from program
        glDetachShader(program, shaders[i]);
        glDeleteShader(shaders[i]);
    }
    glDeleteProgram(program);
}