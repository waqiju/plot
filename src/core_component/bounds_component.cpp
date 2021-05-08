#include "bounds_component.h"
#include "text_component.h"
#include "bounds_getter.h"
#include "plot/plot.h"


Bounds BoundsComponent::kUndefineBounds;


Bounds& BoundsComponent::RetrieveBounds(Component* target)
{
    IBoundsGetter* boundsGetter = dynamic_cast<IBoundsGetter*>(target);
    if (boundsGetter != NULL)
    {
        return boundsGetter->GetBounds();
    }

    return kUndefineBounds;
}


void BoundsComponent::MakeBoundsSymmertric(Transform* transform, Bounds& bounds)
{
    transform->SetLocalPosition(bounds.Center());
    Vector3 size = bounds.Size();
    bounds.min = - size * 0.5f;
    bounds.max = size * 0.5f;
}


BoundsComponent::BoundsComponent(Entity* owner): Component(owner)
{
    name = "Bounds";

	localBounds = Bounds(Vector3::zero, Vector3::zero);
}

Bounds BoundsComponent::WorldBounds()
{
    Matrix4x4 matrix = this->OwnerEntity()->GetTransform()->LocalToWorldMatrix();
	return Bounds(matrix.MultiplyPoint3x4(localBounds.min), matrix.MultiplyPoint3x4(localBounds.max));
}


void BoundsComponent::SyncTargetBounds()
{
    if (target == NULL)
        return;

    localBounds = RetrieveBounds(target);
}