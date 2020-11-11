#pragma once
#include <vector>
#include <map>
#include <string>
#include "pb/prefab.pb.h"
#include "entity/ec.h"
using PbObjectIdMap = std::map<int, const pb::WorldObject*>;


class PrefabLoader
{
public:
    static int GetIntMember(const pb::WorldObject& object, const std::string& key);
    static float GetFloatMember(const pb::WorldObject& object, const std::string& key, const pb::Prefab& prefab);
    static float GetFloatElement(const pb::WorldObject& object, int index, const pb::Prefab& prefab);
	static std::vector<int> GetListMember(const pb::WorldObject& object, const std::string& key, PbObjectIdMap& objectIdMap);

    // load
    static void LoadFromFile(std::string path);

    PrefabLoader(const pb::Prefab* prefab);
    void Load();

private:
    const pb::Prefab* m_Prefab;
    PbObjectIdMap m_ObjectIdMap;

    void BuildObjectIdMap();
    void LoadEntity(const pb::WorldObject& object);
	const pb::WorldObject& GetObject(int id);

    Transform* FindTransform(int id);
};