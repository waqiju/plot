#pragma once
#include <vector>
#include <string>
#include "components_cache.h"


class Entity;
class Component;
class WorldObject;

class World
{
public:
    static World* ActiveWorld();
    static Entity* OriginEntity();
    Entity* CreateEntity();
    Entity* CreateEntity(std::string name);
    std::vector<Entity*> GetRootEntities() { return m_EntityList; }
    void AttachEntity(Entity* toAdd);
    void RemoveFromEntities(Entity* toRemove);
    template<typename T> std::vector<T*> GetComponentsInRootEnities();
    template<typename T> std::vector<T*> GetComponentsInAllEnities();
	void FlushTransform();
    // Find
    WorldObject* FindObject(int id);
    // Cache
    void ClearCache();

private:
    static World* s_ActiveWorld;
    static Entity* s_OriginEntity;
    std::vector<Entity*> m_EntityList;
    ComponentsCache m_Cache;
};


template<typename T> 
std::vector<T*> World::GetComponentsInRootEnities()
{
	std::vector<T*> componentList;
	for (Entity* entity : m_EntityList)
	{
		T* component = entity->template GetComponent<T>();
		if (component != NULL)
		{
			componentList.push_back(component);
		}
	}
	return componentList;
}

template<typename T> 
std::vector<T*> World::GetComponentsInAllEnities()
{
    std::vector<T*> componentList;
    size_t typeCode = typeid(T).hash_code();
    if (m_Cache.Query(typeCode, ComponentsCache::kDefaultExpireTime, componentList))
    {
        return componentList;
    }

    for (Entity* entity : m_EntityList)
    {
        entity->template GetComponentsInChildren<T>(componentList);
    }
    m_Cache.Update(typeCode, componentList);
    return std::move(componentList);
}