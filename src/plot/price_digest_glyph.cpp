#include "price_digest_glyph.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"
#include "plot/plot.h"
#include "core_component/core_component.h"


PriceDigestGlyph* PriceDigestGlyph::Create(Transform* parent, float x, const StockPriceDigest& metadata)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto stockGlyph = entity->AddComponent<PriceDigestGlyph>();
    stockGlyph->Reset(x, metadata);

    entity->GetComponent<Transform>()->SetParent(parent);
    // add bounds component
    auto boundsCp = entity->AddComponent<BoundsComponent>();
    boundsCp->localBounds = stockGlyph->bounds;
    boundsCp->target = stockGlyph;

    return stockGlyph;
}

void PriceDigestGlyph::Reset(float inX, const StockPriceDigest& inMetadata)
{
    x = inX;
    metadata = inMetadata;
    // color
    if (metadata.open == metadata.close)
    {
        if (metadata.close > metadata.previous_close)
        {
            color = Color::red;
        }
        else if (metadata.close < metadata.previous_close)
        {
            color = Color::green;
        }
        else
        {
            color = Color::grey;
        }
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

void PriceDigestGlyph::Render()
{
    RenderRectangle();
    RenderSegment();
}

void PriceDigestGlyph::RenderRectangle()
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

void PriceDigestGlyph::RenderSegment()
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

void PriceDigestGlyph::BatchRender(std::vector<PriceDigestGlyph*>& stockGlyphList)
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

    float pixelSize = CameraHelper::OnePixelSizeInWorld(Application::MainCamera(), Application::screenHeight);
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
            // 特殊情景，如果 open 和 close 非常接近，绘制出来的矩形因为高度为0，
            // 实际没有被像素化。改变 Bounds 的大小，使得可以显示大小为 1-2 像素
            if (std::abs(metadata.open - metadata.close)  < pixelSize)
            {
				auto pairX = MeshVertexPairX(x);
				bounds = Bounds(Vector3(pairX.first, metadata.close, 0), Vector3(pairX.second, metadata.close, 0));
				count = chimera::Segment::GenerateMesh(bounds, sg->color, vertices, colors);
				Matrix4x4Helper::ApplyMatrixForEach(sg->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());
            }
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

std::pair<float, float> PriceDigestGlyph::MeshVertexPairX(float x)
{
	return std::pair<float, float>(x - 0.4f, x + 0.4f);
}
