#pragma once
#include "geometry/geometry.h"
#include "entity/component.h"
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

    void SetLocalPosition(const Vector3& inLocalPosition) 
    { 
        m_LocalPosition = inLocalPosition;
        m_HasChanged = true;
    };
    void SetLocalRotation(const Quaternion& inLocalRotation) 
    { 
        m_LocalRotation = inLocalRotation;
        m_HasChanged = true;
    };
    void SetLocalScale(const Vector3& inLocalScale)
    { 
        m_LocalScale = inLocalScale;
        m_HasChanged = true;
    };

    Matrix4x4 WorldToLocalMatrix() 
    {
        // TODO hierarchy
        return Matrix4x4::TRS(m_LocalPosition, m_LocalRotation, m_LocalScale);
    };
    Matrix4x4 LocalToWorldMatrix() 
    {
        // TODO 
        return Matrix4x4();
    };

private:
	Vector3 m_LocalPosition;
	Quaternion m_LocalRotation;
	Vector3 m_LocalScale;

    bool m_HasChanged;
};