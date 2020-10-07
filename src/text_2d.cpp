#include "text_2d.h"
#include "asset_importer.h"
#include "gl_headers.h"
#include "application/application.h"
#include <vector>



unsigned int Text2D::m_VAO = 0;
unsigned int Text2D::m_TextureID = 0;
unsigned int Text2D::m_VertexBufferID;
unsigned int Text2D::m_UVBufferID;
Material* Text2D::m_Material;


void Text2D::Initialize()
{
    glGenVertexArrays(1, &m_VAO);
    m_TextureID = AssetImporter::LoadDDS("art/Holstein.DDS");
    glGenBuffers(1, &m_VertexBufferID);
    glGenBuffers(1, &m_UVBufferID);

    Shader* shader = new Shader("src/shader/text_2d.vs", "src/shader/text_2d.fs");
    m_Material = new Material(shader);
    m_Material->SetTexture("myTextureSampler", m_TextureID);
}

void Text2D::DrawInScreen(std::string text, int x, int y, int fontSize)
{
    if (m_VAO == 0)
    {
        Initialize();
    }

    Vector2 screenSize = Application::ScreenSize();
    std::vector<Vector2> vertices;
    std::vector<Vector2> uvs;
    for (size_t i = 0; i < text.length(); ++i)
    {
        Vector2 vertexUpLeft = Vector2(x + i * fontSize, y + fontSize) / screenSize * 2.0f - 1.0f;
        Vector2 vertexUpRight = Vector2(x + i * fontSize + fontSize, y + fontSize) / screenSize * 2.0f - 1.0f;
        Vector2 vertexDownRight = Vector2(x + i * fontSize + fontSize, y) / screenSize * 2.0f - 1.0f;
        Vector2 vertexDownLeft = Vector2(x + i * fontSize, y) / screenSize * 2.0f - 1.0f;

        vertices.push_back(vertexUpLeft);
        vertices.push_back(vertexUpRight);
        vertices.push_back(vertexDownLeft);

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

        uvs.push_back(uvUpLeft);
        uvs.push_back(uvUpRight);
        uvs.push_back(uvDownLeft);

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