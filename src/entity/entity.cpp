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
