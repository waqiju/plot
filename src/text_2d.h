#pragma once
#include "material.h"


class Text2D
{
public:
    static void Initialize();
    static void DrawInScreen(std::string text, int x, int y, int fontSize);

private:
    static unsigned int m_VAO;
    static unsigned int m_TextureID;
    static unsigned int m_VertexBufferID;
    static unsigned int m_UVBufferID;
    static Material* m_Material;
};