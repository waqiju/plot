#include "core_component/bounds_component.h"
#include "plot/plot.h"


Bounds BoundsComponent::kUndefineBounds;


Bounds& BoundsComponent::RetrieveBounds(Component* target)
{
    if (typeid(*target) == typeid(Rectangle))
    {
        return dynamic_cast<Rectangle*>(target)->bounds;
    }

    return kUndefineBounds;
}


BoundsComponent::BoundsComponent(Entity* owner): Component(owner)
{
    name = "Bounds";

	localBounds = Bounds(Vector3::zero, Vector3::zero);
}

Bounds BoundsComponent::WorldBounds()
{
    Matrix4x4 matrix = this->GetComponent<Transform>()->LocalToWorldMatrix();
	return Bounds(matrix.MultiplyPoint3x4(localBounds.min), matrix.MultiplyPoint3x4(localBounds.max));
}


void BoundsComponent::SyncTargetBounds()
{
    if (target == NULL)
        return;

    localBounds = RetrieveBounds(target);
}