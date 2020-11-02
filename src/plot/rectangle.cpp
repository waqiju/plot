#include "rectangle.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"


namespace chimera
{

Rectangle* Rectangle::Create(Transform* parent, const Vector3& v0, const Vector3& v1, const Color& color)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto rectangle = entity->AddComponent<Rectangle>();
    rectangle->bounds.min = v0;
    rectangle->bounds.max = v1;
    rectangle->color = color;

    entity->GetComponent<Transform>()->SetParent(parent);
    return rectangle;
}

int Rectangle::GenerateMesh(const Bounds& inBounds, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    /*
        1 -- 3
        |    |
        0 -- 2
    */
    const Vector3 v0 = inBounds.min;
    const Vector3 v1 = Vector3(inBounds.min.x, inBounds.max.y, inBounds.min.z);
    const Vector3 v2 = Vector3(inBounds.max.x, inBounds.min.y, inBounds.max.z);
    const Vector3 v3 = inBounds.max;
    vertices.push_back(v0);
    vertices.push_back(v2);
    vertices.push_back(v1);
    vertices.push_back(v3);
    vertices.push_back(v1);
    vertices.push_back(v2);
    for (int i = 0; i < 6; ++i)
    {
        colors.push_back(inColor);
    }

    return 6;
}

int Rectangle::GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    return GenerateMesh(bounds, color, vertices, colors);
}

void Rectangle::Render()
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

void Rectangle::BatchRender(std::vector<Rectangle*> rectangleList)
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    for (Rectangle* item:rectangleList)
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

} // chimera