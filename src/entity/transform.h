#pragma once
#include "geometry/geometry.h"
#include <string>

class Transform : public Component
{
public:
    Transform(Entity* owner) : Component(owner)
    {
        name = "Transform";
        m_LocalPosition = Vector3::zero;
        m_LocalRotation = Quaternion::identity;
        m_LocalScale = Vector3::one;

        m_HasChanged = true;
    }
    Vector3 LocalPosition() { return m_LocalPosition; }
    Quaternion LocalRotation() { return m_LocalRotation; }
    Vector3 LocalScale() { return m_LocalScale; }
    bool HasChanged() { return m_HasChanged; }

    Vector3 SetLocalPosition(const Vector3& inLocalPosition) 
    { 
        m_LocalPosition = inLocalPosition;
        m_HasChanged = true;
    };
    Vector3 SetLocalRotation(const Quaternion& inLocalRotation) 
    { 
        m_LocalRotation = inLocalRotation;
        m_HasChanged = true;
    };
    Vector3 SetLocalScale(const Vector3& inLocalScale) 
    { 
        m_LocalScale = inLocalScale;
        m_HasChanged = true;
    };

private:
	Vector3 m_LocalPosition;
	Quaternion m_LocalRotation;
	Vector3 m_LocalScale;

    bool m_HasChanged;
};