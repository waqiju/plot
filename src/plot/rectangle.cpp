#include "rectangle.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"


Rectangle* Rectangle::Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto rectangle = entity->AddComponent<Rectangle>();
    rectangle->leftBottom = v1;
    rectangle->rightTop = v2;
    rectangle->color = color;

    entity->GetComponent<Transform>()->SetParent(parent);
    return rectangle;
}

void Rectangle::GenerateMesh(const Vector3& inLeftBottom, const Vector3& inRightTop, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    /*
        1 -- 3
        |    |
        0 -- 2
    */
    const Vector3 v0 = inLeftBottom;
    const Vector3 v1 = Vector3(inLeftBottom.x, inRightTop.y, inLeftBottom.z);
    const Vector3 v2 = Vector3(inRightTop.x, inLeftBottom.y, inRightTop.z);
    const Vector3 v3 = inRightTop;
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

}

void Rectangle::GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors)
{
    GenerateMesh(leftBottom, rightTop, color, vertices, colors);
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
    auto renderer = World::OriginEntity()->GetOrAddComponent<MeshRenderer>();
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
        item->GenerateMesh(vertices, colors);
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