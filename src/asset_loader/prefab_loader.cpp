#include "prefab_loader.h"
#include "object_convertor.h"
#include <assert.h>
#include "entity/ec.h"
#include "plot/plot.h"


int PrefabLoader::GetIntMember(const pb::WorldObject& object, const std::string& key)
{
    for (const pb::Member& member : object.members())
    {
        if (member.key() == key)
        {
            return member.value();
        }
    }
    return 0;
}

float PrefabLoader::GetFloatMember(const pb::WorldObject& object, const std::string& key, const pb::Prefab& prefab)
{
    for (const pb::Member& member : object.members())
    {
        if (member.key() == key)
        {
            return prefab.float_pool()[member.value()];
        }
    }

    return 0;
}

float PrefabLoader::GetFloatElement(const pb::WorldObject& object, int index, const pb::Prefab& prefab)
{
    return prefab.float_pool()[object.members(index).value()];

    return 0;
}

std::vector<int> PrefabLoader::GetListMember(const pb::WorldObject& object, const std::string& key, PbObjectIdMap& objectIdMap)
{
    int objectID = GetIntMember(object, key);

    std::vector<int> list;
    auto listObjectPtr = objectIdMap[objectID];
    assert(listObjectPtr != NULL);
    for (const pb::Member& member : listObjectPtr->members())
    {
        list.push_back(member.value());
    }
    return list;
}

PrefabLoader::PrefabLoader(const pb::Prefab* prefab)
{
    m_Prefab = prefab;

    BuildObjectIdMap();
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
    std::vector<int>&& components = GetListMember(object, "components", m_ObjectIdMap);
	for (int component : components)
	{
		auto componentObj = GetObject(component);
		if (componentObj.type() == "Transform")
		{
			auto transform = entity->GetTransform();
			auto parentId = GetIntMember(componentObj, "parent");
			if (parentId != 0 && World::ActiveWorld()->Find(parentId) != NULL)
			{
				transform->SetParent(World::ActiveWorld()->Find(parentId)->GetTransform());
			}
		}
		else if (componentObj.type() == "Triangle")
		{
			auto triangle = entity->GetOrAddComponent<Triangle>();
            //ConvertBounds(, triangle->bounds);
			triangle->bounds = Bounds(Vector3::zero, Vector3::one);
		}
	}
}

const pb::WorldObject& PrefabLoader::GetObject(int id)
{
    auto objectPtr = m_ObjectIdMap[id];
    assert(objectPtr != NULL);
    return *objectPtr;
}
