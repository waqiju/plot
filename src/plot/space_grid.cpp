#include "space_grid.h"
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "mesh_renderer.h"
#include <vector>
#include "text_2d.h"


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
    auto shader = new Shader("src/shader/color.vs", "src/shader/color.fs");
    auto material = new Material(shader);
    material->SetColor("Color", Color::white);
    renderer->material = material;
}


void SpaceGrid::Render()
{
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
    // vertices
    std::vector<Vector3> vertices;
    int x0 = Mathf::Floor(m_LeftBottom.x);
    int x1 = Mathf::Ceil(m_RightTop.x);
    int y0 = Mathf::Floor(m_LeftBottom.y);
    int y1 = Mathf::Ceil(m_RightTop.y);
    // x0 -> x1
    for (int i = x0; i <= x1; ++i)
    {
        vertices.push_back(Vector3(static_cast<float>(i), static_cast<float>(y0), 0));
        vertices.push_back(Vector3(static_cast<float>(i), static_cast<float>(y1), 0));
    }
    // y0 -> y1
    for (int i = y0; i <= y1; ++i)
    {
        vertices.push_back(Vector3(static_cast<float>(x0), static_cast<float>(i), 0));
        vertices.push_back(Vector3(static_cast<float>(x1), static_cast<float>(i), 0));
    }
    m_Mesh->SetVertices(vertices);

    auto renderer = m_Entity->GetComponent<MeshRenderer>();
    Matrix4x4 mvp = m_Camera->ViewProjectMatrix() * m_Entity->GetComponent<Transform>()->LocalToWorldMatrix();
    renderer->material->SetMatrix("MVP", mvp);
    renderer->Render();

    // label
    Text2D::DrawInScreen("1234ABCD", 100, 100, 50);
}