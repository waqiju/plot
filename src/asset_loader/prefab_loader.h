#pragma once
#include <map>
#include "pb/prefab.pb.h"


class PrefabLoader
{
public:
    PrefabLoader(const pb::Prefab* prefab);
    void Load();

private:
    const pb::Prefab* m_Prefab;
    std::map<int, const pb::WorldObject*> m_ObjectIdMap;

    void BuildObjectIdMap();
    void LoadEntity(const pb::WorldObject& object);
};