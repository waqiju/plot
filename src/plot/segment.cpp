#include "segment.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"


namespace chimera
{

Segment* Segment::Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto segment = entity->AddComponent<Segment>();
    segment->leftBottom = v1;
    segment->rightTop = v2;
    segment->color = color;

    entity->GetComponent<Transform>()->SetParent(parent);
    return segment;
}

void Segment::GenerateMesh(const Vector3& inLeftBottom, const Vector3& inRightTop, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    vertices.push_back(inLeftBottom);
    vertices.push_back(inRightTop);
    colors.push_back(inColor);
    colors.push_back(inColor);
}


void Segment::GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    GenerateMesh(leftBottom, rightTop, color, vertices, colors);
}

void Segment::Render()
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    GenerateMesh(vertices, colors);
    Mesh mesh;
    mesh.SetTopology(MeshTopology::Lines);
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

void Segment::BatchRender(std::vector<Segment*> segmentList)
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    for (Segment* item:segmentList)
    {
        item->GenerateMesh(vertices, colors);
    }
    Mesh mesh;
    mesh.SetTopology(MeshTopology::Lines);
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