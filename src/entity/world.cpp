#include "entity/world.h"
#include "entity/entity.h"

World* World::m_ActiveWorld = NULL;


World* World::ActiveWorld()
{
    if (m_ActiveWorld == NULL)
    {
        m_ActiveWorld = new World();
    }

    return m_ActiveWorld;
}

Entity* World::CreateEntity()
{
	return new Entity(this);
}

void World::AttachEntity(Entity* entity)
{
    m_EntityList.push_back(entity);
}