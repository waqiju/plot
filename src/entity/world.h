#pragma once
#include <vector>

class Entity;
class Component;

class World
{
public:
    static World* ActiveWorld;
    static void CreateOneAsAcitve();

private:
    std::vector<Entity*> m_EntityList;
    std::vector<Component*> m_ComponentList;
};