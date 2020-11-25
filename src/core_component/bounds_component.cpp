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
    if (typeid(*target) == typeid(Triangle))
    {
        return dynamic_cast<Triangle*>(target)->bounds;
    }
    if (typeid(*target) == typeid(chimera::Rectangle))
    {
        return dynamic_cast<chimera::Rectangle*>(target)->bounds;
    }
    if (typeid(*target) == typeid(Pentagram))
    {
        return dynamic_cast<Pentagram*>(target)->bounds;
    }
    // text
    if (typeid(*target) == typeid(TextComponent))
    {
        return dynamic_cast<TextComponent*>(target)->bounds;
    }
    // stock glyph
    if (typeid(*target) == typeid(StockGlyph))
    {
        return dynamic_cast<StockGlyph*>(target)->bounds;
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
    Matrix4x4 matrix = this->GetComponent<Transform>()->LocalToWorldMatrix();
	return Bounds(matrix.MultiplyPoint3x4(localBounds.min), matrix.MultiplyPoint3x4(localBounds.max));
}


void BoundsComponent::SyncTargetBounds()
{
    if (target == NULL)
        return;

    localBounds = RetrieveBounds(target);
}