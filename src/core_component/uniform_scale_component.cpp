#include "core_component/uniform_scale_component.h"
#include "plot/plot.h"
#include "geometry/geometry.h"


UniformScaleComponent::UniformScaleComponent(Entity* owner): Component(owner)
{
    name = "UniformScale";
}

void UniformScaleComponent::ResizeBounds()
{
    if (target == NULL || mode == UniformScaleMode::None)
        return;

    Bounds bounds = RetrieveBounds();

    Vector3 scaling = this->GetTransform()->LocalToWorldMatrix().DecomposeScaling();
    switch (mode)
    {
        case UniformScaleMode::FixedWidthInWorldSpace:
        {
			Vector3 boundSize = bounds.Size();
            float sizeInWorldY = boundSize.y * scaling.y;
            if (Mathf::IsZero(sizeInWorldY))
                return;

            float expandY = boundSize.x * scaling.x / sizeInWorldY;
            bounds.Expand(Vector3(1, expandY, 1));
            break;
        }
    }

    AssignBounds(bounds);
}

Bounds UniformScaleComponent::RetrieveBounds()
{
    if (typeid(target) == typeid(Rectangle*))
    {
        return static_cast<Rectangle*>(target)->bounds;
    }

    return Bounds();
}

void UniformScaleComponent::AssignBounds(const Bounds& bounds)
{
    if (typeid(target) == typeid(Rectangle))
    {
		static_cast<Rectangle*>(target)->bounds = bounds;
    }
}