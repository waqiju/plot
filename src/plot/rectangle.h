#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "entity/ec.h"
#include <vector>


namespace chimera
{

class Rectangle : public Component, public IPrefabConvertor
{
public:
    static constexpr int VerticesCount() { return 6; } 
    static Rectangle* Create(Transform* parent, const Vector3& v0, const Vector3& v1, const Color& color);
    static int GenerateMesh(const Bounds& inBounds, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors);
    static void BatchRender(std::vector<Rectangle*> rectangleList);

    Rectangle(Entity* owner): Component(owner) {}

    Bounds bounds;
    Color color = Color::white;

    int GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors);
    void Render();

    // Prefab
    void Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader) override;
};

} // chimera