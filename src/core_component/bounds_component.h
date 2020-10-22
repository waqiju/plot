#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


class BoundsComponent: public Component
{
public:
    BoundsComponent(Entity* owner);
    const Bounds& LocalBounds() const { return m_LocalBounds; };
    const Bounds& WorldBounds() const { return m_WorldBounds; };

    void SetLocalBounds(const Bounds& bounds);
    void SetLocalBounds(const Vector3& min, const Vector3& max);
    void MarkAsDirty();

private:
    Bounds m_LocalBounds;
    Bounds m_WorldBounds;

    void TransformChangedHandler(Transform* transform);
};