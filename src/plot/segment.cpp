#include "segment.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"


namespace chimera
{

Segment* Segment::Create(Transform* parent, const Vector3& v0, const Vector3& v1, const Color& color)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto segment = entity->AddComponent<Segment>();
    segment->bounds.min = v0;
    segment->bounds.max = v1;
    segment->color = color;

    entity->GetComponent<Transform>()->SetParent(parent);
    return segment;
}

int Segment::GenerateMesh(const Bounds& inBounds, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    vertices.push_back(inBounds.min);
    vertices.push_back(inBounds.max);
    colors.push_back(inColor);
    colors.push_back(inColor);
    return 2;
}


int Segment::GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    return GenerateMesh(bounds, color, vertices, colors);
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
    auto renderer = m_OwerEntity->GetOrAddComponent<MeshRenderer>();
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
        int count = item->GenerateMesh(vertices, colors);
        Matrix4x4Helper::ApplyMatrixForEach(item->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());
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