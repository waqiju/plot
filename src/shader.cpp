#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // read file
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // write string
        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Read shader file failed!" << std::endl;
    }
    // compile shaders
    unsigned int vertexID, fragmentID;
    const char* vertexCodeCStr = vertexCode.c_str();
    const char* fragmentCodeCStr = fragmentCode.c_str();
    vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vertexCodeCStr, NULL);
    glCompileShader(vertexID);
    CheckCompileErrors(vertexID, "Compile vertex shader failed:");
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID, 1, &fragmentCodeCStr, NULL);
    glCompileShader(fragmentID);
    CheckCompileErrors(fragmentID, "Compile fragment shader failed:");
    // link
    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);
    glLinkProgram(ID);
    CheckLinkErrors(ID, "Link program failed:");
    // clean
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

void Shader::Use()
{
    glUseProgram(ID);
}


void Shader::CheckCompileErrors(GLuint shaderID, std::string errorHint)
{
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
        std::cout << errorHint << "\n" << infoLog << std::endl;
    }
}

void Shader::CheckLinkErrors(GLuint programID, std::string errorHint)
{
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(programID, 1024, NULL, infoLog);
        std::cout << errorHint << "\n" << infoLog << std::endl;
    }
}