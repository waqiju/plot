#include "space_grid.h"
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "mesh_renderer.h"
#include <vector>
#include "text_2d.h"
#include <iostream>
#include <sstream>


SpaceGrid::SpaceGrid(Camera* camera)
{
    m_Camera = camera;
    m_LeftBottom = Vector3::zero;
    m_RightTop = Vector3::zero;
    m_Entity = World::ActiveWorld()->CreateEntity();
    m_Entity->name = "SpaceGrid";
    auto renderer = m_Entity->AddComponent<MeshRenderer>();
    // mesh and material
    m_Mesh = new Mesh();
    m_Mesh->SetTopology(MeshTopology::Lines);
    renderer->mesh = m_Mesh;
    auto shader = new Shader("src/shader/color_vertex.vs", "src/shader/color_vertex.fs");
    auto material = new Material(shader);
    material->SetColor("ColorTint", Color::white);
    renderer->material = material;
}


void SpaceGrid::Render()
{
    // interval
    float intervalX, intervalY;
    CalculateInterval(10, intervalX, intervalY);

    // grid plane, normal(0, 0, -1)
    Plane plane = Plane(Vector3::zero, Vector3(0, 0, -1));
    Ray ray = m_Camera->ViewportPointToRay(Vector3(-1, -1, 1));
    if (!Physics::Raycast(ray, plane, m_LeftBottom))
    {
        return;
    }
    ray = m_Camera->ViewportPointToRay(Vector3(1, 1, 1));
    if (!Physics::Raycast(ray, plane, m_RightTop))
    {
        return;
    }
    // vertics colors
    static Color gradeZeroColor = Color(1, 1, 1, 0.75);
    static Color gradeOneColor = Color(1, 1, 1, 0.4);
    static Color gradeTwoColor = Color(1, 1, 1, 0.15);
    static Color gradeThreeColor = Color(1, 1, 1, 0.05);
    // numberic
    float maxUlpX = intervalX * 0.01;
    float maxUlpY = intervalY * 0.01;
    // vertices
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    float x0 = Mathf::ToFloorUnit(m_LeftBottom.x, intervalX, maxUlpX);
    float x1 = Mathf::ToCeilUnit(m_RightTop.x, intervalX, maxUlpX);
    float y0 = Mathf::ToFloorUnit(m_LeftBottom.y, intervalY, maxUlpY);
    float y1 = Mathf::ToCeilUnit(m_RightTop.y, intervalY, maxUlpY);
    // x0 -> x1
    for (float i = x0; i <= x1; i+=intervalX)
    {
        vertices.push_back(Vector3(static_cast<float>(i), static_cast<float>(y0), 0));
        vertices.push_back(Vector3(static_cast<float>(i), static_cast<float>(y1), 0));
        if (Mathf::Equal(i, 0, maxUlpX))
        {
            colors.push_back(gradeZeroColor);
            colors.push_back(gradeZeroColor);
        }
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalX * 4, maxUlpX), maxUlpX))
        {
            colors.push_back(gradeOneColor);
            colors.push_back(gradeOneColor);
        }
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalX * 2, maxUlpX), maxUlpX))
        {
            colors.push_back(gradeTwoColor);
            colors.push_back(gradeTwoColor);
        }
        else
        {
            colors.push_back(gradeThreeColor);
            colors.push_back(gradeThreeColor);
        }
    }
    // y0 -> y1
    for (float i = y0; i <= y1; i+=intervalY)
    {
        vertices.push_back(Vector3(static_cast<float>(x0), static_cast<float>(i), 0));
        vertices.push_back(Vector3(static_cast<float>(x1), static_cast<float>(i), 0));
        if (Mathf::Equal(i, 0, maxUlpY))
        {
            colors.push_back(gradeZeroColor);
            colors.push_back(gradeZeroColor);
        }
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalY * 4, maxUlpY), maxUlpY))
        {
            colors.push_back(gradeOneColor);
            colors.push_back(gradeOneColor);
        }
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalY * 2, maxUlpY), maxUlpY))
        {
            colors.push_back(gradeTwoColor);
            colors.push_back(gradeTwoColor);
        }
        else
        {
            colors.push_back(gradeThreeColor);
            colors.push_back(gradeThreeColor);
        }
    }
    m_Mesh->SetVertices(vertices);
    m_Mesh->SetColors(colors);

    auto renderer = m_Entity->GetComponent<MeshRenderer>();
    // 渲染
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderer->camera = m_Camera;
    renderer->Render();
    glDisable(GL_BLEND);

    // label
    std::stringstream stream;
    stream.precision(3);
    stream.unsetf(std::ios::showpoint);
    for (float i = x0; i <= x1; i+=intervalX)
    {
        if (Mathf::Equal(i, 0, maxUlpX))
            i = 0;
        if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalY * 4, maxUlpX), maxUlpX))
        {
            stream.str("");
            stream << i;
            Text2D::DrawInWorld(stream.str(), Vector3(i, m_LeftBottom.y, 0), 18);
        }
    }
    for (float i = y0; i <= y1; i+=intervalY)
    {
        if (Mathf::Equal(i, 0, maxUlpY))
            i = 0;
        if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalY * 4, maxUlpY), maxUlpY))
        {
            stream.str("");
            stream << i;
            Text2D::DrawInWorld(stream.str(), Vector3(m_LeftBottom.x, i, 0), 18);
        }
    }
}


void SpaceGrid::CalculateInterval(int pixel, float& intervalX, float& intervalY)
{
    Vector3 v0(1000, 1000, 0);
    auto matrix = m_Camera->ViewProjectMatrix();
    Vector3 zeroInScreen = m_Camera->ViewportToScreenPoint(matrix.MultiplyPoint(Vector3::zero));

    Vector3 nowGrade = v0;
    for (int i = 0; i < 1000; ++i)
    {
        Vector3 v1 = m_Camera->ViewportToScreenPoint(matrix.MultiplyPoint(v0));
        Vector3 distance = Vector3::Abs(v1 - zeroInScreen);
        if (distance.x < pixel && distance.y < pixel)
            return;

        if (distance.x >= pixel)
            intervalX = v0.x;
        if (distance.y >= pixel)
            intervalY = v0.y;
        // 下降
        if (i % 2 == 0)
            v0 = nowGrade * 0.5;
        else
            nowGrade = v0 = nowGrade * 0.1;
    }
    intervalX = intervalY = 1;
}