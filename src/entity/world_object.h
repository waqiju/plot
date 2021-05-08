/*
 * World中对象的公共基类
 */
#pragma once
#include <string>
#include "object_id.h"


class Entity;
class Transform;


enum class LifecycleFlag: int
{
    Default = 0,
    Created = 1,
    DestroyStart = 1 << 1,
    DestroyEnd = 1 << 2,

};

inline LifecycleFlag& operator&=(LifecycleFlag& f1, const LifecycleFlag f2)
{
	f1 = static_cast<LifecycleFlag>((int)(f1) & (int)(f2));
	return f1;
}

inline LifecycleFlag& operator|=(LifecycleFlag& f1, const LifecycleFlag f2)
{
	f1 = static_cast<LifecycleFlag>((int)(f1) | (int)(f2));
	return f1;
}

class WorldObject
{
public:
    ObjectID id;
    std::string name = "Unnamed";

    int GetInstanceID();
    int GetHashCode();
	virtual std::string ToString();
    // 构建
    WorldObject();
    // 析构
    virtual ~WorldObject() {id.Unregister();}
    virtual void Destroy() {};
	// Utility
	Entity* ToEntity();
	Transform* ToTransform();

public:
    LifecycleFlag Lifecycle() { return m_Lifecycle; }
	bool InDestroy() { return (static_cast<int>(m_Lifecycle) & static_cast<int>(LifecycleFlag::DestroyStart)) || (static_cast<int>(m_Lifecycle) & static_cast<int>(LifecycleFlag::DestroyEnd)); }
    bool HasCreated() { return (static_cast<int>(m_Lifecycle) & static_cast<int>(LifecycleFlag::Created)); }

protected:
    void SetFlagDestroyStart() { m_Lifecycle |= LifecycleFlag::DestroyStart; }
    void SetFlagDestroyEnd() { m_Lifecycle |= LifecycleFlag::DestroyEnd; }

private:
    static int s_InstanceCount;
	LifecycleFlag m_Lifecycle = LifecycleFlag::Created;

};