#include "core_component/bounds_component.h"


BoundsComponent::BoundsComponent(Entity* owner): Component(owner)
{
    name = "Bounds";
    this->GetComponent<Transform>()->onChanged += event_handler(this, &BoundsComponent::TransformChangedHandler);

	m_LocalBounds = Bounds(Vector3::zero, Vector3::zero);
	MarkAsDirty();
}

void BoundsComponent::SetLocalBounds(const Bounds& bounds)
{
    m_LocalBounds = bounds;
    MarkAsDirty();
}

void BoundsComponent::SetLocalBounds(const Vector3& min, const Vector3& max)
{
    m_LocalBounds.min = min;
    m_LocalBounds.max = max;
    MarkAsDirty();
}

void BoundsComponent::MarkAsDirty()
{
    Matrix4x4 matrix = this->GetComponent<Transform>()->LocalToWorldMatrix();
    m_WorldBounds.min = matrix.MultiplyPoint3x4(m_LocalBounds.min);
    m_WorldBounds.max = matrix.MultiplyPoint3x4(m_LocalBounds.max);
}

void BoundsComponent::TransformChangedHandler(Transform* transform)
{
    Matrix4x4 matrix = transform->LocalToWorldMatrix();
    m_WorldBounds.min = matrix.MultiplyPoint3x4(m_LocalBounds.min);
    m_WorldBounds.max = matrix.MultiplyPoint3x4(m_LocalBounds.max);
}