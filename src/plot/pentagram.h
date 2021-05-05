#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "entity/ec.h"
#include "core_component/core_component.h"
#include <vector>


class Pentagram : public Component, public IBoundsGetter
{
public:
    static Pentagram* Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color);
    static int GenerateMesh(const Bounds& inBounds, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors);
    static void BatchRender(std::vector<Pentagram*> pentagramList);

    Pentagram(Entity* owner): Component(owner) {}

    Bounds bounds;
    Color color = Color::white;

    int GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors);
    void Render();
    Bounds& GetBounds() { return bounds; }
};