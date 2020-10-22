#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


class BoundsComponent: public Component
{
public:
    BoundsComponent(Entity* owner);
    const Bounds& LocalBounds() const { return m_LocalBounds; };
    Bounds WorldBounds();

    void SetLocalBounds(const Bounds& bounds);
    void SetLocalBounds(const Vector3& min, const Vector3& max);

private:
    Bounds m_LocalBounds;
};