#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


class BoundsComponent: public Component
{
public:
    static Bounds kUndefineBounds;
    static Bounds& RetrieveBounds(Component* target);
    static void McakeBoundsSymmertric(Transform* transform, Bounds& bounds);

    BoundsComponent(Entity* owner);

    Component* target;
    Bounds localBounds;

    Bounds WorldBounds();
    void SyncTargetBounds();
};