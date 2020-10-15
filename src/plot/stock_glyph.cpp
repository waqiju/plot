#include "stock_glyph.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"


StockGlyph* StockGlyph::Create(Transform* parent, const KChart& chart)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto stockGlyph = entity->AddComponent<StockGlyph>();
    stockGlyph->Reset(chart);

    entity->GetComponent<Transform>()->SetParent(parent);
    return stockGlyph;
}

void StockGlyph::Reset(KChart inKChart)
{
    chart = inKChart;
    // color
    if (chart.open() == chart.close())
    {
        color = Color::grey;
    }
    else if (chart.open() < chart.close())
    {
        color = Color::red;
    }
    else
    {
        color = Color::green;
    }
    // leftBottom, rightTop
}

void StockGlyph::Render()
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

void StockGlyph::BatchRender(std::vector<StockGlyph*> stockGlyphList)
{
    // TODO

}