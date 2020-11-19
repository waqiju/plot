#include "entity/entity.h"
#include "entity/world.h"


// Entity::Entity() : Entity(World::ActiveWorld())
// {}

Entity::Entity(World* onwerWorld)
{
    m_OnwerWorld = onwerWorld;
}

bool Entity::Active()
{
    return m_Active;
}

void Entity::SetActive(bool status)
{
    m_Active = status;
}

Entity::~Entity()
{
    for (auto pair : m_ComponentMap)
    {
        auto component = pair.second;
        delete component;
    }
}

void Entity::Destroy()
{
    if (InDestroy())
        return;
    SetFlagDestroyStart();

    for (auto pair : m_ComponentMap)
    {
        auto component = pair.second;
        component->Destroy();
    }

    WorldObject::Destroy();
    SetFlagDestroyEnd();
}