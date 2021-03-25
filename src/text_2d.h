#pragma once
#include "material.h"
#include "geometry/geometry.h"


class Text2D
{
public:
    static void Initialize();
    static void DrawInWorld(std::string text, Vector3 position, int fontSize);
    static void DrawInViewport(std::string text, float x, float y, int fontSize);
    static void DrawInScreen(std::string text, int x, int y, int fontSize);

    static Bounds CalculateBounds(std::string text, Vector3 position, int fontSize);

private:
    static unsigned int m_VAO;
    static unsigned int m_TextureID;
    static unsigned int m_VertexBufferID;
    static unsigned int m_UVBufferID;
    static Material* m_Material;
};