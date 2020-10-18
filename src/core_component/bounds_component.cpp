#include "core_component/bounds_component.h"


BoundsComponent::BoundsComponent(Entity* owner): Component(owner), Bounds(Vector3::zero, Vector3::zero)
{
    name = "Bounds";
}
