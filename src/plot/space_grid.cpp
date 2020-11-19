#include "space_grid.h"
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "mesh_renderer.h"
#include <vector>
#include "text_2d.h"
#include <iostream>
#include <sstream>

const float SpaceGrid::s_Ticks[18] = { 1000, 500.0, 100.0, 50.0, 10.0, 5.0, 1.0, 0.5, 0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0001, 5e-05, 1e-05, 5e-06 };


SpaceGrid::SpaceGrid(Camera* camera, Transform* transform)
{
    m_Camera = camera;
    m_Transform = transform;
    m_LeftBottom = Vector3::zero;
    m_RightTop = Vector3::zero;
    auto renderer = m_Transform->AddComponent<MeshRenderer>();
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
    RenderLine();
    RenderLabel();
}

void SpaceGrid::RenderLine()
{
    // interval
    int tickGradeX, tickGradeY;
    CalculateInterval(10, tickGradeX, tickGradeY);
    float intervalX = GetTick(tickGradeX);
    float intervalY = GetTick(tickGradeY);
    float intervalXMinus1 = GetTick(tickGradeX-1);
    float intervalXMinus2 = GetTick(tickGradeX-2);
    float intervalYMinus1 = GetTick(tickGradeY-1);
    float intervalYMinus2 = GetTick(tickGradeY-2);

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
    m_LeftBottom = m_Transform->WorldToLocalMatrix().MultiplyPoint3x4(m_LeftBottom);
    m_RightTop = m_Transform->WorldToLocalMatrix().MultiplyPoint3x4(m_RightTop);
    // vertics colors
    static Color gradeZeroColor = Color(1, 1, 1, 0.75f);
    static Color gradeOneColor = Color(1, 1, 1, 0.4f);
    static Color gradeTwoColor = Color(1, 1, 1, 0.15f);
    static Color gradeThreeColor = Color(1, 1, 1, 0.05f);
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
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalXMinus2, maxUlpX), maxUlpX))
        {
            colors.push_back(gradeOneColor);
            colors.push_back(gradeOneColor);
        }
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalXMinus1, maxUlpX), maxUlpX))
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
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalYMinus2, maxUlpY), maxUlpY))
        {
            colors.push_back(gradeOneColor);
            colors.push_back(gradeOneColor);
        }
        else if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalYMinus1, maxUlpY), maxUlpY))
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

    auto renderer = m_Transform->GetComponent<MeshRenderer>();
    // 渲染
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderer->camera = m_Camera;
    renderer->Render();
    glDisable(GL_BLEND);
}

void SpaceGrid::RenderLabel()
{
	// interval
	int tickGradeX, tickGradeY;
	CalculateInterval(10, tickGradeX, tickGradeY);
	float intervalX = GetTick(tickGradeX);
	float intervalY = GetTick(tickGradeY);
	float intervalXMinus1 = GetTick(tickGradeX - 1);
	float intervalXMinus2 = GetTick(tickGradeX - 2);
	float intervalYMinus1 = GetTick(tickGradeY - 1);
	float intervalYMinus2 = GetTick(tickGradeY - 2);

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
	m_LeftBottom = m_Transform->WorldToLocalMatrix().MultiplyPoint3x4(m_LeftBottom);
	m_RightTop = m_Transform->WorldToLocalMatrix().MultiplyPoint3x4(m_RightTop);
	// numberic
	float maxUlpX = intervalX * 0.01;
	float maxUlpY = intervalY * 0.01;
	float x0 = Mathf::ToFloorUnit(m_LeftBottom.x, intervalX, maxUlpX);
	float x1 = Mathf::ToCeilUnit(m_RightTop.x, intervalX, maxUlpX);
	float y0 = Mathf::ToFloorUnit(m_LeftBottom.y, intervalY, maxUlpY);
	float y1 = Mathf::ToCeilUnit(m_RightTop.y, intervalY, maxUlpY);

    // label
    std::stringstream stream;
    stream.precision(3);
    stream.unsetf(std::ios::showpoint);
    for (float i = x0; i <= x1; i+=intervalX)
    {
        if (Mathf::Equal(i, 0, maxUlpX))
            i = 0;
        if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalXMinus1, maxUlpX), maxUlpX))
        {
            stream.str("");
            stream << i;
            Vector3 textWorldPosition = m_Transform->LocalToWorldMatrix().MultiplyPoint3x4(Vector3(i, m_LeftBottom.y, 0));
            Text2D::DrawInWorld(stream.str(), textWorldPosition, 18);
        }
    }
    for (float i = y0; i <= y1; i+=intervalY)
    {
        if (Mathf::Equal(i, 0, maxUlpY))
            i = 0;
        if (Mathf::Equal(i, Mathf::ToNearUnit(i, intervalYMinus1, maxUlpY), maxUlpY))
        {
            stream.str("");
            stream << i;
            Vector3 textWorldPosition = m_Transform->LocalToWorldMatrix().MultiplyPoint3x4(Vector3(m_LeftBottom.x, i, 0));
            Text2D::DrawInWorld(stream.str(), textWorldPosition, 18);
        }
    }
}

void SpaceGrid::CalculateInterval(int pixel, int& tickGradeX, int& tickGradeY)
{
    Vector3 v0;
    auto matrix = m_Camera->ViewProjectMatrix() * m_Transform->LocalToWorldMatrix();
    Vector3 zeroInScreen = m_Camera->ViewportToScreenPoint(matrix.MultiplyPoint(Vector3::zero));

    tickGradeX = tickGradeY = 0;
    for (size_t i = 0; i < sizeof(s_Ticks); ++i)
    {
        v0.x = v0.y = s_Ticks[i];

        Vector3 v1 = m_Camera->ViewportToScreenPoint(matrix.MultiplyPoint(v0));
        Vector3 distance = Vector3::Abs(v1 - zeroInScreen);
        if (distance.x < pixel && distance.y < pixel)
            return;

        if (distance.x >= pixel)
            tickGradeX = i;
        if (distance.y >= pixel)
            tickGradeY = i;
    }
}

float SpaceGrid::GetTick(int index)
{
    if (index < 0)
        index = 0;
    if (index > sizeof(s_Ticks))
        index = sizeof(s_Ticks) - 1;

    return s_Ticks[index];
}