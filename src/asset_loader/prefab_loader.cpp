﻿#include "prefab_loader.h"
#include "object_convertor.h"
#include "component_convertor.h"
#include <assert.h>
#include "entity/ec.h"
#include "plot/plot.h"
#include "common/common.h"


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

double PrefabLoader::GetDoubleMember(const pb::WorldObject& object, const std::string& key, const pb::Prefab& prefab)
{
    for (const pb::Member& member : object.members())
    {
        if (member.key() == key)
        {
            return prefab.double_pool()[member.value()];
        }
    }

    return 0;
}

std::string PrefabLoader::GetStringMember(const pb::WorldObject& object, const std::string& key, const pb::Prefab& prefab)
{
    for (const pb::Member& member : object.members())
    {
        if (member.key() == key)
        {
            return prefab.string_pool()[member.value()];
        }
    }

    return "";
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

void PrefabLoader::LoadFromFile(std::string path)
{
    std::string content;
    FileHelper::Read(path, content);
    pb::Prefab prefab;
    prefab.ParseFromString(content);

    auto loader = PrefabLoader(&prefab);
    loader.Load();
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

    // 只加载第一个 Entity
    // const pb::WorldObject& root = m_Prefab->world_object_pool()[0];
    // LoadEntity(root);

    // 遍历依次加载所有的 Entity
    for (const pb::WorldObject& object: m_Prefab->world_object_pool())
    {
       if (object.type() == "Entity" && ObjectID::Find(object.id()) == NULL)
       {
           LoadEntity(object);
       }
    }
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
        AddComponentToEntity(*entity, componentObj, *this);
	}
    // children, 这里是用 child 的 parent 字段来构建 hierarchy，
    // 而非利用 parent 的 children，出于实现简单的原因了
    // 实现在 Load() 中
}

const pb::WorldObject& PrefabLoader::GetObject(int id)
{
    auto objectPtr = m_ObjectIdMap[id];
    assert(objectPtr != NULL);
    return *objectPtr;
}

WorldObject* PrefabLoader::FindObject(int id)
{
    if (id == 0)
        return NULL;

    // return World::ActiveWorld()->FindObject(id);
    return ObjectID::Find(id);
}