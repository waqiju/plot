#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


class BoundsComponent: public Component, public Bounds
{
public:
    BoundsComponent(Entity* owner);
};