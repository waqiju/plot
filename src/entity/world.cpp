#include "entity/world.h"
#include "entity/entity.h"
#include "entity/transform.h"

World* World::s_ActiveWorld = NULL;
Entity* World::s_OriginEntity = NULL;


World* World::ActiveWorld()
{
    if (s_ActiveWorld == NULL)
    {
        s_ActiveWorld = new World();
    }

    return s_ActiveWorld;
}

Entity* World::OriginEntity()
{
    if (s_OriginEntity == NULL)
    {
        // 位于原点
        s_OriginEntity = ActiveWorld()->CreateEntity();
    }

    return s_OriginEntity;
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