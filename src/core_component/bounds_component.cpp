#include "core_component/bounds_component.h"


BoundsComponent::BoundsComponent(Entity* owner): Component(owner)
{
    name = "Bounds";

	m_LocalBounds = Bounds(Vector3::zero, Vector3::zero);
}

Bounds BoundsComponent::WorldBounds()
{
    Matrix4x4 matrix = this->GetComponent<Transform>()->LocalToWorldMatrix();
	return Bounds(matrix.MultiplyPoint3x4(m_LocalBounds.min), matrix.MultiplyPoint3x4(m_LocalBounds.max));
}

void BoundsComponent::SetLocalBounds(const Bounds& bounds)
{
    m_LocalBounds = bounds;
}

void BoundsComponent::SetLocalBounds(const Vector3& min, const Vector3& max)
{
    m_LocalBounds.min = min;
    m_LocalBounds.max = max;
}
