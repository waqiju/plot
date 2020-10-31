#include "triangle.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"


Triangle* Triangle::Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color, bool isFlipY)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto triangle = entity->AddComponent<Triangle>();
    triangle->bounds.min = v1;
    triangle->bounds.max = v2;
    triangle->color = color;
    triangle->isFlipY = isFlipY;

    entity->GetComponent<Transform>()->SetParent(parent);
    return triangle;
}


std::vector<Vector3> kTriangleVertices = {
    Vector3(0, 0.4330127018922193f, 0),
    Vector3(-0.5f, -0.4330127018922193f, 0),
    Vector3(0.5f, -0.4330127018922193f, 0),
};


int Triangle::GenerateMesh(const Bounds& inBounds, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors, bool isFlipY)
{
    /*
        0
       | |
      1   2
    */
    Vector3 center = inBounds.Center();
    Vector3 scaling = inBounds.Size();

    if (!isFlipY)
    {
        vertices.push_back(center + kTriangleVertices[0] * scaling);
        vertices.push_back(center + kTriangleVertices[1] * scaling);
        vertices.push_back(center + kTriangleVertices[2] * scaling);
    }
    else
    {
        vertices.push_back(center + kTriangleVertices[0] * Vector3(1, -1, 1) * scaling);
        vertices.push_back(center + kTriangleVertices[1] * Vector3(1, -1, 1) * scaling);
        vertices.push_back(center + kTriangleVertices[2] * Vector3(1, -1, 1) * scaling);
    }

    for (int i = 0; i < 3; ++i)
    {
        colors.push_back(inColor);
    }

    return 3;
}

int Triangle::GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    return GenerateMesh(bounds, color, vertices, colors, isFlipY);
}

void Triangle::Render()
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    GenerateMesh(vertices, colors);
    Mesh mesh;
    mesh.SetVertices(vertices);
    mesh.SetColors(colors);
    // material
    auto shader = ResourceManager::LoadShader("src/shader/color_vertex.vs", "src/shader/color_vertex.fs", "", "color_vertex");
    Material material = Material(shader);
    material.SetColor("ColorTint", Color::white);
    // renderer
    auto renderer = m_OwerEntity->GetOrAddComponent<MeshRenderer>();
    renderer->mesh = &mesh;
    renderer->material = &material;
    renderer->camera = Application::MainCamera();
    renderer->Render();
}

void Triangle::BatchRender(std::vector<Triangle*> triangleList)
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    for (Triangle* item:triangleList)
    {
        int count = item->GenerateMesh(vertices, colors);
        Matrix4x4Helper::ApplyMatrixForEach(item->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());
    }
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
    renderer->camera = Application::MainCamera();
    renderer->Render();
}