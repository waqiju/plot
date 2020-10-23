#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


enum class UniformScaleMode
{
    None,
    FixedWidthInWorldSpace,
};


class UniformScaleComponent: public Component
{
public:
    UniformScaleComponent(Entity* owner);

    Component* target;
    UniformScaleMode mode;

    void ResizeBounds();
};