#pragma once
#include <vector>
#include <map>
#include <typeinfo>
#include <cassert>
#include "entity/world_object.h"

class Component;

class Entity : public WorldObject
{
public:
    bool Actived();
    template<typename T> T* AddComponent();
    template<typename T> T* GetComponent();

private:
    void SetActive(bool status);

private:
    bool m_Actived = false;
    std::map<size_t, Component*> m_ComponentMap;
};


template<typename T>
inline T* Entity::AddComponent()
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
inline T* Entity::GetComponent()
{
    size_t typeCode = typeid(T).hash_code();
    if (m_ComponentMap.find(typeCode) != m_ComponentMap.end())
    {
        return m_ComponentMap[typeCode];
    }

    return NULL;
}
