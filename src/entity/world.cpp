#include "entity/world.h"
#include "entity/entity.h"
#include "entity/transform.h"

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
    Entity* entity = new Entity(this);
    entity->AddComponent<Transform>();
    m_EntityList.push_back(entity);
	return entity;
}

void World::FlushTransform()
{
    for (auto transform : GetComponentsInRootEnities<Transform>())
    {
        transform->Flush();
    }
}

// void World::AttachEntity(Entity* entity)
// {
//     m_EntityList.push_back(entity);
// }