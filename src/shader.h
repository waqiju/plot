#pragma once
#include <glad/glad.h>
#include <string>


class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Use();

private:
    std::string m_VertexPath, m_FragmentPath;

    void CheckCompileErrors(GLuint shaderID, std::string errorHint);
    void CheckLinkErrors(GLuint programID, std::string errorHint);
};