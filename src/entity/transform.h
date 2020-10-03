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
        m_IsTrsDirty = true;
        // Hierarchy
        m_Parent = NULL;
        m_Children.clear();
    }
    Vector3 LocalPosition() { return m_LocalPosition; }
    Quaternion LocalRotation() { return m_LocalRotation; }
    Vector3 LocalScale() { return m_LocalScale; }
    bool HasChanged() { return m_IsTrsDirty; }

    void SetLocalPosition(const Vector3& inLocalPosition) 
    { 
        m_LocalPosition = inLocalPosition;
        m_IsTrsDirty = true;
    };
    void SetLocalRotation(const Quaternion& inLocalRotation) 
    { 
        m_LocalRotation = inLocalRotation;
        m_IsTrsDirty = true;
    };
    void SetLocalScale(const Vector3& inLocalScale)
    { 
        m_LocalScale = inLocalScale;
        m_IsTrsDirty = true;
    };
    void SetTrsMatrix(const Matrix4x4& inMatrix);
    void Flush(bool includeChildren=true, bool force=false);

    Matrix4x4 WorldToLocalMatrix() 
    {
        // TODO 
        return Matrix4x4();
    };
    Matrix4x4 LocalToWorldMatrix() 
    {
        return m_LocalToWorld;
    };

    Vector3 Right()
    {
        Matrix4x4 m = LocalToWorldMatrix();
        return Vector3(m[0][0], m[0][1], m[0][2]);
    };

    Vector3 Up()
    {
        Matrix4x4 m = LocalToWorldMatrix();
        return Vector3(m[1][0], m[1][1], m[1][2]);
    };

    Vector3 Forward()
    {
        Matrix4x4 m = LocalToWorldMatrix();
        return Vector3(m[2][0], m[2][1], m[2][2]);
    };

private:
    // TRS
	Vector3 m_LocalPosition;
	Quaternion m_LocalRotation;
	Vector3 m_LocalScale;
    Matrix4x4 m_LocalToWorld;
    // Hierarchy
    Transform* m_Parent;
    std::vector<Transform*> m_Children;    

    bool m_IsTrsDirty;
};