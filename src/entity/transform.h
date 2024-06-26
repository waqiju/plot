#pragma once
#include "geometry/geometry.h"
#include "entity/component.h"
#include <string>
#include "common/event.h"
#include "asset_loader/prefab_convertor.h"


class Transform : public Component, public IPrefabConvertor
{
public:
    Transform(Entity* owner) : Component(owner), onChanged(this)
    {
        name = "Transform";
        m_LocalPosition = Vector3::zero;
        m_LocalRotation = Quaternion::identity;
        m_LocalScale = Vector3::one;
        MarkAsDirty();
        // Hierarchy
        m_Parent = NULL;
        m_Children.clear();
    }
    Vector3 LocalPosition() { return m_LocalPosition; }
    Vector3 Position() { return m_LocalToWorld.DecomposePosition(); }
    Quaternion LocalRotation() { return m_LocalRotation; }
    Vector3 LocalScale() { return m_LocalScale; }
    bool HasChanged() { return m_IsTrsDirty; }

    void SetLocalPosition(const Vector3& inLocalPosition) 
    { 
        m_LocalPosition = inLocalPosition;
        MarkAsDirty();
    };
    void SetLocalRotation(const Quaternion& inLocalRotation) 
    { 
        m_LocalRotation = inLocalRotation;
        MarkAsDirty();
    };
    void SetLocalScale(const Vector3& inLocalScale)
    { 
        m_LocalScale = inLocalScale;
        MarkAsDirty();
    };
    void SetTrsMatrix(const Matrix4x4& inMatrix);
    void SetPosition(const Vector3& position);

    void SetParent(Transform* parent);
    size_t ChildCount() { return m_Children.size(); }
    Transform* GetChild(size_t index) { return m_Children[index]; }

    event<Transform> onChanged;
    void Flush(bool includeChildren=true, bool force=false);
    void MarkAsDirty(bool flush=true);

    Matrix4x4 WorldToLocalMatrix() const
    {
        // It's expensive
        return m_LocalToWorld.Inverse();
    };
    const Matrix4x4& LocalToWorldMatrix() const 
    {
        return m_LocalToWorld;
    };

    Vector3 Right()
    {
        Matrix4x4 m = LocalToWorldMatrix();
        return Vector3::Normalize(Vector3(m[0][0], m[0][1], m[0][2]));
    };

    Vector3 Up()
    {
        Matrix4x4 m = LocalToWorldMatrix();
        return Vector3::Normalize(Vector3(m[1][0], m[1][1], m[1][2]));
    };

    Vector3 Forward()
    {
        Matrix4x4 m = LocalToWorldMatrix();
        return Vector3::Normalize(Vector3(m[2][0], m[2][1], m[2][2]));
    };

    const std::vector<Transform*>& Children()
    {
        return m_Children;
    }

    std::string ToString();

    ~Transform() override;
    void Destroy() override;

    // Prefab
    void Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader) override;

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