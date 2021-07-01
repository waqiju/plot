#include "cursor_sight.h"
#include "camera.h"
#include "camera_helper.h"
#include "text_2d.h"
#include "application/application.h"
#include "resource/resource_manager.h"
#include "plot/rectangle.h"
#include "mesh_renderer.h"
#include <iostream>
using namespace chimera;


const Color CursorSight::kDefaultColor = Color(0.9f, 0.9f, 0.9f, 0.75f);


CursorSight::CursorSight()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VertexBufferID);
    glGenBuffers(1, &m_DirectionBufferID);

    Shader* shader = new Shader("src/shader/cursor_sight.vs", "src/shader/cursor_sight.fs");
    m_Material = new Material(shader);
    shader = new Shader("src/shader/color_vertex_in_viewport.vs", "src/shader/cursor_sight.fs");
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
    if (m_DirectionBufferID)
    {
        glDeleteBuffers(1, &m_DirectionBufferID);
    }
}

void CursorSight::Render()
{
    if (style == SightStyle::None)
    {
        return;
    }
    // 鼠标在窗口外
    if (CameraHelper::IsMousePositionOutOfScreen())
    {
        return;
    }
     Vector3 mousePosition = Input::MousePosition();
     Vector3 mousePositionInViewport = Camera::ScreenToViewportPoint(mousePosition);
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

    // 显示当前鼠标位置的坐标读数
    RenderAsixReadout();
}


void CursorSight::RenderAsixReadout()
{
    const int kFontSize = 16;

    Vector3 worldPosition;
    Vector3 mousePosition = Input::MousePosition();
    Vector3 mousePositionInViewport = Camera::ScreenToViewportPoint(mousePosition);
    Ray ray = Application::MainCamera()->ScreenPointToRay(mousePosition);
    Physics::Raycast(ray, Plane::XyPlane, worldPosition);
    // Plot Main Panel 下的坐标
    Transform* mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
    Vector3 localPosition = mainPanelTr->WorldToLocalMatrix().MultiplyPoint3x4(worldPosition);

    // 准备文本内容
    float y = localPosition.y;
    std::string label = Mathf::TickNumberToString(y, 2);
    // Y 轴背景
    // Mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    int capcity = chimera::Rectangle::VerticesCount();
    vertices.reserve(capcity);
    colors.reserve(capcity);

    // Background
    Vector3 backgroundMin = Vector3(-1, mousePositionInViewport.y, mousePositionInViewport.z);
    Bounds textBackgroundBounds = Text2D::CalculateBoundsInViewport(label + " ", backgroundMin, kFontSize);
    Rectangle::GenerateMesh(textBackgroundBounds, Color::kBackgroundWhite, vertices, colors);

    Mesh mesh;
    mesh.SetVertices(vertices);
    mesh.SetColors(colors);
    // material
    auto shader = ResourceManager::LoadShader("src/shader/color_vertex.vs", "src/shader/color_vertex.fs", "", "color_vertex");
    Material material = Material(shader);
    material.SetColor("ColorTint", Color::white);
    // renderer
    auto renderer = World::OriginEntity()->GetOrAddComponent<MeshRenderer>();
    renderer->mesh = &mesh;
    renderer->material = &material;
    renderer->camera = nullptr;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderer->Render();

    // Y 轴 Label
    Text2D::DrawInViewport(label, -1, mousePositionInViewport.y, kFontSize);
}
