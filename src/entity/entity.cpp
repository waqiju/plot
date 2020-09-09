#include "entity/entity.h"
#include "entity/world.h"


Entity::Entity() : Entity(World::ActiveWorld())
{
}

Entity::Entity(World* onwerWorld)
{
    m_OnwerWorld = onwerWorld;
    onwerWorld->AttachEntity(this);
}

bool Entity::Active()
{
    return m_Active;
}

World* Entity::OnwerWorld()
{
    return m_OnwerWorld;
}

void Entity::SetActive(bool status)
{
    m_Active = status;
}
