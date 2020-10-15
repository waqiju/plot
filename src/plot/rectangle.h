#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "entity/ec.h"
#include <vector>


class Rectangle : public Component
{
public:
    static Rectangle* Create(Transform* parent, const Vector3& v1, const Vector3& v2, const Color& color);
    static void GenerateMesh(const Vector3& inLeftBottom, const Vector3& inRightTop, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors);
    static void BatchRender(std::vector<Rectangle*> rectangleList);

    Rectangle(Entity* owner): Component(owner) {}

    Vector3 leftBottom = Vector3::zero, rightTop = Vector3::zero;
    Color color = Color::white;

    void GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors);
    void Render();
};