#include "core_component/uniform_scale_component.h"


UniformScaleComponent::UniformScaleComponent(Entity* owner): Component(owner)
{
    name = "UniformScale";
}

void UniformScaleComponent::ResizeBounds()
{
    // TODO
}