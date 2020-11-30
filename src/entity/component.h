#pragma once
#include <vector>
#include "entity/world_object.h"

class Entity;
class Transform;

class Component: public WorldObject
{
public:
    Component(Entity* owner);
    Entity* OwnerEntity();
    Transform* GetTransform();
    bool Enabled();

    template<typename T> T* AddComponent() { return OwnerEntity()->template AddComponent<T>(); }
    template<typename T> T* GetComponent() { return OwnerEntity()->template GetComponent<T>(); }
    template<typename T> std::vector<T*> GetComponentsInChildren()
    {
        return OwnerEntity()->template GetComponentsInChildren<T>();
    }
    template<typename T> void GetComponentsInChildren(std::vector<T*>& componentList)
    {
        OwnerEntity()->template GetComponentsInChildren<T>(componentList);
    }

private:
    void Enable(bool status);

protected:
    Entity* m_OwerEntity = NULL;
    bool m_Enabled = true;
};