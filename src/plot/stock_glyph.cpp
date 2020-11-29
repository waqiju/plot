#include "stock_glyph.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"
#include "plot/plot.h"
#include "core_component/core_component.h"


StockGlyph* StockGlyph::Create(Transform* parent, float x, const StockMetadata& metadata)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto stockGlyph = entity->AddComponent<StockGlyph>();
    stockGlyph->Reset(x, metadata);

    entity->GetComponent<Transform>()->SetParent(parent);
    // add bounds component
    auto boundsCp = entity->AddComponent<BoundsComponent>();
    boundsCp->localBounds = stockGlyph->bounds;
    boundsCp->target = stockGlyph;

    return stockGlyph;
}

void StockGlyph::Reset(float inX, const StockMetadata& inMetadata)
{
    x = inX;
    metadata = inMetadata;
    // color
    if (metadata.open == metadata.close)
    {
        color = Color::grey;
    }
    else if (metadata.open < metadata.close)
    {
        color = Color::red;
    }
    else
    {
        color = Color::green;
    }
    color.a = 0.75f;
	auto vertexPairX = MeshVertexPairX(x);
    bounds.min = Vector3(vertexPairX.first, metadata.low, 0);
    bounds.max = Vector3(vertexPairX.second, metadata.high, 0);
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
    float min = Mathf::Min(metadata.open, metadata.close);
    float max = Mathf::Max(metadata.open, metadata.close);
    Bounds bounds = Bounds(Vector3(x - 0.45, min, 0), Vector3(x + 0.45, max, 0));
    chimera::Rectangle::GenerateMesh(bounds, color, vertices, colors);
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
    Bounds bounds(Vector3(x, metadata.low, 0), Vector3(x, metadata.high, 0));
    chimera::Segment::GenerateMesh(bounds, color, vertices, colors);
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
        int capcity = chimera::Rectangle::VerticesCount() * stockGlyphList.size();
        vertices.reserve(capcity);
        colors.reserve(capcity);

        for (auto sg:stockGlyphList)
        {
            float x = sg->x;
            auto& metadata = sg->metadata;
            float min = Mathf::Min(metadata.open, metadata.close);
            float max = Mathf::Max(metadata.open, metadata.close);
            auto pairX = MeshVertexPairX(x);
            Bounds bounds = Bounds(Vector3(pairX.first, min, 0), Vector3(pairX.second, max, 0));
            int count = chimera::Rectangle::GenerateMesh(bounds, sg->color, vertices, colors);
            Matrix4x4Helper::ApplyMatrixForEach(sg->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        renderer->Render();
    }

    // segment
    {
        // mesh
        std::vector<Vector3> vertices;
        std::vector<Color> colors;
        int capcity = chimera::Segment::VerticesCount() * stockGlyphList.size();
        vertices.reserve(capcity);
        colors.reserve(capcity);

        for (auto sg:stockGlyphList)
        {
            float x = sg->x;
            auto& metadata = sg->metadata;
            // 防止和 Rectangle 的透明叠加，所以拆为两个 Segment
            float low2 = std::min(metadata.open, metadata.close);
            float high2 = std::max(metadata.open, metadata.close);

            Bounds bounds = Bounds(Vector3(x, metadata.low, 0), Vector3(x, low2, 0));
            int count = chimera::Segment::GenerateMesh(bounds, sg->color, vertices, colors);
            Matrix4x4Helper::ApplyMatrixForEach(sg->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());

            bounds = Bounds(Vector3(x, high2, 0), Vector3(x, metadata.high, 0));
            count = chimera::Segment::GenerateMesh(bounds, sg->color, vertices, colors);
            Matrix4x4Helper::ApplyMatrixForEach(sg->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        renderer->Render();
    }
}

std::pair<float, float> StockGlyph::MeshVertexPairX(float x)
{
	return std::pair<float, float>(x - 0.4f, x + 0.4f);
}
