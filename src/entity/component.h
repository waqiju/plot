#pragma once
#include <vector>
#include "entity/world_object.h"

class Entity;

class Component: public WorldObject
{
public:
    Component(Entity* owner);
    Entity* OnwerEntity();
    bool Enabled();

    template<typename T> T* AddComponent() { return OnwerEntity()->template AddComponent<T>(); }
    template<typename T> T* GetComponent() { return OnwerEntity()->template GetComponent<T>(); }

private:
    void Enable(bool status);

private:
    Entity* m_OwerEntity = NULL;
    bool m_Enabled = true;
};