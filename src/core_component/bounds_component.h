#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


class BoundsComponent: public Component
{
public:
    static Bounds kUndefineBounds;
    static Bounds& RetrieveBounds(Component* target);

    BoundsComponent(Entity* owner);

    Component* target;

    const Bounds& LocalBounds() const { return m_LocalBounds; };
    void SetLocalBounds(const Bounds& bounds);
    void SetLocalBounds(const Vector3& min, const Vector3& max);
    Bounds WorldBounds();

    void AlignBounds();

private:
    Bounds m_LocalBounds;
};