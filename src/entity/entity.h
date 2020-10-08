#pragma once
#include <vector>
#include <map>
#include <typeinfo>
#include <cassert>
#include "entity/world_object.h"


class Component;
class World;


class Entity : public WorldObject
{
public:
    // Entity();
    Entity(World* onwerWorld);

    bool Active();
    World* OnwerWorld();

    template<typename T> T* AddComponent();
    template<typename T> T* GetComponent();
    template<typename T> T* GetOrAddComponent();

private:
    void SetActive(bool status);

private:
    bool m_Active = true;
    World* m_OnwerWorld = NULL;
    std::map<size_t, Component*> m_ComponentMap;
};


template<typename T>
T* Entity::AddComponent()
{
    T* component = new T(this);

    size_t typeCode = typeid(T).hash_code();
    if (m_ComponentMap.find(typeCode) != m_ComponentMap.end())
    {
        assert(false);
    }
    m_ComponentMap[typeCode] = component;

    return component;
}

template<typename T>
T* Entity::GetComponent()
{
    size_t typeCode = typeid(T).hash_code();
    if (m_ComponentMap.find(typeCode) != m_ComponentMap.end())
    {
        return (T*)m_ComponentMap[typeCode];
    }

    return NULL;
}

template<typename T>
T* Entity::GetOrAddComponent()
{
    T* component = this->template GetComponent<T>();
    if (component == NULL)
    {
        component = this->template AddComponent<T>();
    }
    return component;
}