#pragma once
#include <vector>
#include "entity/world_object.h"

class Entity;
class Transform;

class Component: public WorldObject
{
public:
    Component(Entity* owner);
    Entity* OnwerEntity();
    Transform* GetTransform();
    bool Enabled();

    template<typename T> T* AddComponent() { return OnwerEntity()->template AddComponent<T>(); }
    template<typename T> T* GetComponent() { return OnwerEntity()->template GetComponent<T>(); }
    template<typename T> std::vector<T*> GetComponentsInChildren()
    {
        return OnwerEntity()->template GetComponentsInChildren<T>();
    }
    template<typename T> void GetComponentsInChildren(std::vector<T*>& componentList)
    {
        OnwerEntity()->template GetComponentsInChildren<T>(componentList);
    }

private:
    void Enable(bool status);

protected:
    Entity* m_OwerEntity = NULL;
    bool m_Enabled = true;
};