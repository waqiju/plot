#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "entity/ec.h"
#include <vector>


class Triangle : public Component
{
public:
    static Triangle* Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color, bool isFlipY=false);
    static int GenerateMesh(const Bounds& inBounds, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors, bool isFlipY);
    static void BatchRender(std::vector<Triangle*> triangleList);

    Triangle(Entity* owner): Component(owner) {}

    Bounds bounds;
    Color color = Color::white;
    bool isFlipY = false;

    int GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors);
    void Render();
};