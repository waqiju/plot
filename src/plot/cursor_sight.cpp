#include "cursor_sight.h"
#include "camera.h"
#include "application/application.h"
#include "resource/resource_manager.h"
#include <iostream>


const Color CursorSight::kDefaultColor = Color(0.9f, 0.9f, 0.9f, 0.75f);


CursorSight::CursorSight()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VertexBufferID);
    glGenBuffers(1, &m_DirectionBufferID);

    Shader* shader = new Shader("src/shader/cursor_sight.vs", "src/shader/cursor_sight.fs");
    m_Material = new Material(shader);
}

CursorSight::~CursorSight()
{
    if (m_Material != nullptr)
    {
        delete m_Material;
    }
    if (m_VAO)
    {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VertexBufferID)
    {
        glDeleteBuffers(1, &m_VertexBufferID);
    }
}

void CursorSight::Render()
{
    if (style == SightStyle::None)
    {
        return;
    }
    // 鼠标在窗口外
    Vector3 mousePosition = Input::MousePosition();
    Vector3 mousePositionInViewport = Camera::ScreenToViewportPoint(mousePosition);
    if (mousePositionInViewport.x < -1 || mousePositionInViewport.y < -1 || mousePositionInViewport.x > 1 || mousePositionInViewport.y > 1)
    {
        return;
    }
    // 构建顶点
    std::vector<Vector2> vertices;
    std::vector<float> clipDirection;
    if (style == SightStyle::Horizontal || style == SightStyle::Cross)
    {
        float y = mousePositionInViewport.y;
        vertices.push_back(Vector2(-1, y));
        vertices.push_back(Vector2(1, y));
        // >= 0 代表横向，见 cursor_sight.fs
        clipDirection.push_back(1);
        clipDirection.push_back(1);
    }
    if (style == SightStyle::Vertical || style == SightStyle::Cross)
    {
        float x = mousePositionInViewport.x;
        vertices.push_back(Vector2(x, -1));
        vertices.push_back(Vector2(x, 1));
        clipDirection.push_back(-1);
        clipDirection.push_back(-1);
    }
    // 绘制数据准备
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector2), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_DirectionBufferID);
    glBufferData(GL_ARRAY_BUFFER, clipDirection.size() * sizeof(float), &clipDirection[0], GL_STATIC_DRAW);
    // Shader
    m_Material->SetFloat("ScreenWidth", Application::screenWidth);
    m_Material->SetFloat("ScreenHeight", Application::screenHeight);
    m_Material->SetColor("ColorTint", color);
    m_Material->Use();
    // 1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 2nd attribute buffer: direction
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_DirectionBufferID);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // draw call
    glDrawArrays(GL_LINES, 0, vertices.size());

    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    // std::cout<< mousePosition.ToString()<<std::endl;
    // std::cout<< mousePositionInViewport.ToString()<<std::endl;
}