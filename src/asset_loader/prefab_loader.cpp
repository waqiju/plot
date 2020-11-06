#include "prefab_loader.h"
#include <assert.h>


PrefabLoader::PrefabLoader(const pb::Prefab* prefab)
{
    m_Prefab = prefab;
}

void PrefabLoader::BuildObjectIdMap()
{
    m_ObjectIdMap.clear();

    for (auto& object : m_Prefab->world_object_pool())
    {
        m_ObjectIdMap[object.id()] = &object;
    }
}

void PrefabLoader::Load()
{
	if (m_Prefab->world_object_pool().size() <= 0)
		return;

    const pb::WorldObject& root = m_Prefab->world_object_pool()[0];
    LoadEntity(root);
}

void PrefabLoader::LoadEntity(const pb::WorldObject& object)
{
    assert(object.type() == "Entity");
    auto entity = World::ActiveWorld()->CreateEntity(object.name());
    entity->id = object.id();
    // components
    std::vector<int> components = ;
    for (const pb::Member& member : prefabRoot.members())
    {
        if (member.key() == "components")
        {

        }
    }
}