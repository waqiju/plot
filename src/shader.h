#pragma once
#include <glad/glad.h>
#include <string>


class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexpath, const char* fragmentPath);

    void Use();

private:
    void CheckCompileErrors(GLuint shaderID, std::string errorHint);
    void CheckLinkErrors(GLuint programID, std::string errorHint);
};