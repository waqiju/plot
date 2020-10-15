#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "entity/ec.h"
#include <vector>


namespace chimera
{

class Segment : public Component
{
public:
    static Segment* Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color);
    static void BatchRender(std::vector<Segment*> segmentList);

    Segment(Entity* owner): Component(owner) {}

    Vector3 leftBottom = Vector3::zero, rightTop = Vector3::zero;
    Color color = Color::white;

    void GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors);
    void Render();
};

} // chimera