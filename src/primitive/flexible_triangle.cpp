#include "flexible_triangle.h"
#include "procedural_shape.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "graphic/graphic.h"
#include "application/application.h"
#include "core_component/core_component.h"


FlexibleTriangle::FlexibleTriangle(Entity* owner): Component(owner)
{
    name = "FlexibleTriangle";

    mode = FlexibleShapeMode::FixedWidth;
    center = Vector3::zero;
    height = width = 0;
}

void FlexibleTriangle::Render()
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    // Vector3 v0 = Vector3(-1, -1, 0);
    // Vector3 v1 = Vector3(1, 1, 0);
    Bounds bound = ProceduralShape::CalculateBounds(Application::MainCamera()->WorldToCameraMatrix(), center, mode, width, height);
    int verticsCount = ProceduralShape::GenerateTriangle(bound.min, bound.max, vertices);
    ProceduralShape::AppendColors(Color::white, verticsCount, colors);
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