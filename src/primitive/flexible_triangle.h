#pragma once
#include "flexible_shape_mode.h"
#include "entity/ec.h"
#include "geometry/geometry.h"


class FlexibleTriangle : public Component
{
public:
    FlexibleTriangle(Entity* owner);

    FlexibleShapeMode  mode;
    Vector3 center;
    float height;
    float width;

    void Render();
};