#include "stock_glyph.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"
#include "plot/plot.h"
#include "core_component/core_component.h"


StockGlyph* StockGlyph::Create(Transform* parent, float x, const KChart& chart)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto stockGlyph = entity->AddComponent<StockGlyph>();
    stockGlyph->Reset(x, chart);

    entity->GetComponent<Transform>()->SetParent(parent);
    // bounds
	auto bounds = entity->AddComponent<BoundsComponent>()->WorldBounds();
    bounds.min = stockGlyph->leftBottom;
    bounds.max = stockGlyph->rightTop;
    return stockGlyph;
}

void StockGlyph::Reset(float inX, const KChart& inKChart)
{
    x = inX;
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
    leftBottom = Vector3(x - 0.45f, chart.low(), 0);
    rightTop = Vector3(x + 0.45f, chart.high(), 0);
}

void StockGlyph::Render()
{
    RenderRectangle();
    RenderSegment();
}

void StockGlyph::RenderRectangle()
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    float min = Mathf::Min(chart.open(), chart.close());
    float max = Mathf::Max(chart.open(), chart.close());
    Vector3 v0 = Vector3(x - 0.45, min, 0);
    Vector3 v1 = Vector3(x + 0.45, max, 0);
    Rectangle::GenerateMesh(v0, v1, color, vertices, colors);
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

void StockGlyph::RenderSegment()
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    Vector3 v0 = Vector3(x, chart.low(), 0);
    Vector3 v1 = Vector3(x, chart.high(), 0);
    chimera::Segment::GenerateMesh(v0, v1, color, vertices, colors);
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

void StockGlyph::BatchRender(std::vector<StockGlyph*>& stockGlyphList)
{
    // rectangle
    {
        // mesh
        std::vector<Vector3> vertices;
        std::vector<Color> colors;
        for (auto sg:stockGlyphList)
        {
            float x = sg->x;
            auto& chart = sg->chart;
            float min = Mathf::Min(chart.open(), chart.close());
            float max = Mathf::Max(chart.open(), chart.close());
            Vector3 v0 = Vector3(x - 0.45, min, 0);
            Vector3 v1 = Vector3(x + 0.45, max, 0);
            Rectangle::GenerateMesh(v0, v1, sg->color, vertices, colors);
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

    // segment
    {
        // mesh
        std::vector<Vector3> vertices;
        std::vector<Color> colors;
        for (auto sg:stockGlyphList)
        {
            float x = sg->x;
            auto& chart = sg->chart;
            Vector3 v0 = Vector3(x, chart.low(), 0);
            Vector3 v1 = Vector3(x, chart.high(), 0);
            chimera::Segment::GenerateMesh(v0, v1, sg->color, vertices, colors);
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
}