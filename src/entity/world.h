#pragma once
#include <vector>

class Entity;
class Component;

class World
{
public:
    static World* ActiveWorld();
    Entity* CreateEntity();
    void AttachEntity(Entity* entity);
    template<typename T> std::vector<T*> GetComponentsInEnities();

private:
    static World* m_ActiveWorld;
    std::vector<Entity*> m_EntityList;
};


template<typename T> 
std::vector<T*> World::GetComponentsInEnities()
{
	std::vector<T*> componentList;
	for (Entity* entity : m_EntityList)
	{
		// TODO use GetComponentsInChilid
		T* component = entity->template GetComponent<T>();
		if (component != NULL)
		{
			componentList.push_back(component);
		}
	}
	return componentList;
}