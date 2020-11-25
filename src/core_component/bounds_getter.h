#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


class IBoundsGetter
{
public:
    virtual Bounds& GetBounds() = 0;
};