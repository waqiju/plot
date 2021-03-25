#include "text_2d.h"
#include "asset_importer.h"
#include "gl_headers.h"
#include "application/application.h"
#include <vector>
#include "camera.h"
#include "camera_helper.h"


unsigned int Text2D::m_VAO = 0;
unsigned int Text2D::m_TextureID = 0;
unsigned int Text2D::m_VertexBufferID;
unsigned int Text2D::m_UVBufferID;
Material* Text2D::m_Material;


void Text2D::Initialize()
{
    glGenVertexArrays(1, &m_VAO);
    Texture2D* texture = AssetImporter::LoadTexture2D("art/consolas.png", true);
    m_TextureID = texture->ID;
    glGenBuffers(1, &m_VertexBufferID);
    glGenBuffers(1, &m_UVBufferID);

    Shader* shader = new Shader("src/shader/text_2d.vs", "src/shader/text_2d.fs");
    m_Material = new Material(shader);
    m_Material->SetTexture("FontTextureSampler", m_TextureID);
    m_Material->SetColor("ColorTint", Color::white);
}

void Text2D::DrawInScreen(std::string text, int x, int y, int fontSize)
{
    Vector2 screenSize = Application::ScreenSize();
    Vector2 viewportPosition = Vector2(x, y) / screenSize * 2.0f - 1.0f;
    DrawInViewport(text, viewportPosition.x, viewportPosition.y, fontSize);
}

void Text2D::DrawInWorld(std::string text, Vector3 position, int fontSize)
{
    Vector3 viewportPosition = Application::MainCamera()->ViewProjectMatrix().MultiplyPoint(position);
    DrawInViewport(text, viewportPosition.x, viewportPosition.y, fontSize);
}

void Text2D::DrawInViewport(std::string text, float x, float y, int fontSize)
{
    if (text.empty())
    {
        return;
    }

    if (m_VAO == 0)
    {
        Initialize();
    }

    float fontWidthInViewport = 2.0f / Application::screenWidth * fontSize;
    float characterSpacing = fontWidthInViewport * 0.6f;
    float fontHeightInViewport = 2.0f / Application::screenHeight * fontSize;
    std::vector<Vector2> vertices;
    std::vector<Vector2> uvs;
    int capcity = text.length() * 6;
    vertices.reserve(capcity);
    uvs.reserve(capcity);

    for (size_t i = 0; i < text.length(); ++i)
    {
        Vector2 vertexUpLeft = Vector2(x + i * characterSpacing, y + fontHeightInViewport);
        Vector2 vertexUpRight = Vector2(x + i * characterSpacing + fontWidthInViewport, y + fontHeightInViewport);
        Vector2 vertexDownRight = Vector2(x + i * characterSpacing + fontWidthInViewport, y);
        Vector2 vertexDownLeft = Vector2(x + i * characterSpacing, y);

        vertices.push_back(vertexDownLeft);
        vertices.push_back(vertexUpRight);
        vertices.push_back(vertexUpLeft);

        vertices.push_back(vertexDownRight);
        vertices.push_back(vertexUpRight);
        vertices.push_back(vertexDownLeft);

        char character = text[i];
        float uvX = (character % 16) / 16.0f;
        float uvY = (character / 16) / 16.0f;

        float uvOffset = 1.0f / 16.0f;
        Vector2 uvUpLeft = Vector2(uvX, uvY);
        Vector2 uvUpRight = Vector2(uvX + uvOffset, uvY);
        Vector2 uvDownRight = Vector2(uvX + uvOffset, uvY+uvOffset);
        Vector2 uvDownLeft = Vector2(uvX, uvY+uvOffset);

        uvs.push_back(uvDownLeft);
        uvs.push_back(uvUpRight);
        uvs.push_back(uvUpLeft);

        uvs.push_back(uvDownRight);
        uvs.push_back(uvUpRight);
        uvs.push_back(uvDownLeft);
    }
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector2), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_UVBufferID);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), &uvs[0], GL_STATIC_DRAW);

    // 绘制指令
    m_Material->Use();

    // 1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 2nd attribute buffer: uvs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_UVBufferID);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw call
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glDisable(GL_BLEND);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

Bounds Text2D::CalculateBounds(std::string text, Vector3 position, int fontSize)
{
    Bounds bounds;
    bounds.min = position;
    // max
    float height = CameraHelper::OnePixelSizeInWorld(Application::MainCamera(), Application::screenHeight) * fontSize;
    float x = position.x + height * text.size() * 0.6f;
    float y = position.y + height;
    bounds.max = Vector3(x, y, position.z);

    return bounds;
}