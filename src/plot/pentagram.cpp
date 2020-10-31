#include "pentagram.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"
#include "core_component/core_component.h"


Pentagram* Pentagram::Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto pentagram = entity->AddComponent<Pentagram>();
    auto tr = entity->GetComponent<Transform>();
    pentagram->bounds.min = v1;
    pentagram->bounds.max = v2;
    pentagram->color = color;

    BoundsComponent::McakeBoundsSymmertric(tr, pentagram->bounds);
    tr->SetParent(parent);
    return pentagram;
}


std::vector<Vector3> kPentagramVerticesA = {
Vector3(3.061616997868383e-17f, 0.5f, 0),
Vector3(0.47552825814757677f, 0.1545084971874737f, 0),
Vector3(0.29389262614623657f, -0.4045084971874737f, 0),
Vector3(-0.2938926261462365f, -0.4045084971874737f, 0),
Vector3(-0.4755282581475768f, 0.15450849718747364f, 0),
};

std::vector<Vector3> kPentagramVerticesB = {
Vector3(-3.508300897953876e-17f, -0.19098300562505f, 0),
Vector3(-0.18163563200133778f, -0.0590169943749466f, 0),
Vector3(-0.1122569941448948f, 0.15450849718747162f, 0),
Vector3(0.11225699414489483f, 0.15450849718747162f, 0),
Vector3(0.18163563200133775f, -0.05901699437494662f, 0),
};

int Pentagram::GenerateMesh(const Bounds& inBounds, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    Vector3 center = inBounds.Center();
    Vector3 scaling = inBounds.Size();

    // a0 - b4 - a3
    vertices.push_back(center + kPentagramVerticesA[0] * scaling);
    vertices.push_back(center + kPentagramVerticesB[4] * scaling);
    vertices.push_back(center + kPentagramVerticesA[3] * scaling);
    // a0 - a2 - b1
    vertices.push_back(center + kPentagramVerticesA[0] * scaling);
    vertices.push_back(center + kPentagramVerticesA[2] * scaling);
    vertices.push_back(center + kPentagramVerticesB[1] * scaling);
    // a1 - b0 - a4
    vertices.push_back(center + kPentagramVerticesA[1] * scaling);
    vertices.push_back(center + kPentagramVerticesB[0] * scaling);
    vertices.push_back(center + kPentagramVerticesA[4] * scaling);

    for (int i = 0; i < 9; ++i)
    {
        colors.push_back(inColor);
    }

    return 9;
}

int Pentagram::GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    return GenerateMesh(bounds, color, vertices, colors);
}

void Pentagram::Render()
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

void Pentagram::BatchRender(std::vector<Pentagram*> pentagramList)
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    for (Pentagram* item:pentagramList)
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