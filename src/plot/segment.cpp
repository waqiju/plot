#include "segment.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"
#include "line_stipple.h"


namespace chimera
{


void RenderVertices(std::vector<Vector3>& vertices, std::vector<Color>& colors, int stipplePattern);


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
    std::sort(segmentList.begin(), segmentList.end(), [](auto a, auto b) {return a->stipplePattern < b->stipplePattern; });
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    Segment* previous = nullptr;
    if (segmentList.size() > 0)
    {
        previous = segmentList[0];
    }

    for (Segment* item:segmentList)
    {
        if (item->stipplePattern != previous->stipplePattern && vertices.size() > 0)
        {
            RenderVertices(vertices, colors, previous->stipplePattern);
            vertices.clear();
            colors.clear();
        }

        int count = item->GenerateMesh(vertices, colors);
        Matrix4x4Helper::ApplyMatrixForEach(item->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());
        previous = item;
    }
    if (vertices.size() > 0)
    {
        RenderVertices(vertices, colors, previous->stipplePattern);
    }
}

void RenderVertices(std::vector<Vector3>& vertices, std::vector<Color>& colors, int stipplePattern)
{
    // stipple
    std::vector<Vector3>* toDrawVerticesPointer;
    std::vector<Color>* toDrawColorPointer;
    std::vector<Vector3> stippleVertices;
    std::vector<Color> stippleColors;
    if (stipplePattern == 0 || stipplePattern == kStipplePatternDefault)
    {
        toDrawVerticesPointer = &vertices;
        toDrawColorPointer = &colors;
    }
    else
    {
        ApplyLineStipple(vertices, colors, stippleVertices, stippleColors, stipplePattern);
        toDrawVerticesPointer = &stippleVertices;
        toDrawColorPointer = &stippleColors;
    }

    Mesh mesh;
    mesh.SetTopology(MeshTopology::Lines);
    mesh.SetVertices(*toDrawVerticesPointer);
    mesh.SetColors(*toDrawColorPointer);
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

void Segment::Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader)
{
    assert(pbComponentObj.type() == "Segment");
    auto segment = entity.GetOrAddComponent<chimera::Segment>();
    int boundsObjectID = PrefabLoader::GetIntMember(pbComponentObj, "bounds");
    if (boundsObjectID)
    {
        ConvertBounds(loader.GetObject(boundsObjectID), segment->bounds, loader.Prefab());
    }
    int colorID = PrefabLoader::GetIntMember(pbComponentObj, "color");
    if (colorID)
    {
        ConvertColor(loader.GetObject(colorID), segment->color, loader.Prefab());
    }
    this->stipplePattern = PrefabLoader::GetIntMember(pbComponentObj, "stipplePattern");
}

} // chimera