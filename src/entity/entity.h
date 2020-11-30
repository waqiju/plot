#pragma once
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <cassert>
#include <string>
#include "entity/world_object.h"
#include "entity/transform.h"


class Component;
class World;


class Entity : public WorldObject
{
public:
    // Entity();
    Entity(World* onwerWorld);

    bool Active();
    inline World* OnwerWorld() { return m_OnwerWorld; }
    inline Transform* GetTransform() 
    {
        if (m_Transform == NULL)
        {
            m_Transform = this->GetComponent<Transform>(); 
        }
        return m_Transform;
    }

    template<typename T> T* AddComponent();
    template<typename T> T* GetComponent();
    template<typename T> T* GetOrAddComponent();
    template<typename T> std::vector<T*> GetComponentsInChildren();
    template<typename T> void GetComponentsInChildren(std::vector<T*>& componentList);
    const std::unordered_map<size_t, Component*>& GetComponentMap() { return m_ComponentMap; }

    ~Entity() override;
    void Destroy() override;

    std::string tag;

private:
    void SetActive(bool status);

private:
    bool m_Active = true;
    World* m_OnwerWorld = NULL;
    Transform* m_Transform = NULL;
    std::unordered_map<size_t, Component*> m_ComponentMap;
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
    auto it = m_ComponentMap.find(typeCode);
    if (it != m_ComponentMap.end())
    {
        return (T*)it->second;
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

template<typename T>
std::vector<T*> Entity::GetComponentsInChildren()
{
    std::vector<T*> componentList;
    this->template GetComponentsInChildren<T>(componentList);
    return componentList;
}

template<typename T> 
void Entity::GetComponentsInChildren(std::vector<T*>& componentList)
{
    T* component = this->template GetComponent<T>();
    if (component != NULL)
    {
        componentList.push_back(component);
    }

    Transform* thisTransform = this->GetTransform();
    if (thisTransform == NULL)
        return;
    for (Transform* child: thisTransform->Children())
    {
        child->GetComponentsInChildren<T>(componentList);
    }
}