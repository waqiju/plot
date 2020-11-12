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
    // member
    static int GetIntMember(const pb::WorldObject& object, const std::string& key);
    static float GetFloatMember(const pb::WorldObject& object, const std::string& key, const pb::Prefab& prefab);
    static double GetDoubleMember(const pb::WorldObject& object, const std::string& key, const pb::Prefab& prefab);
    static std::string GetStringMember(const pb::WorldObject& object, const std::string& key, const pb::Prefab& prefab);
	static std::vector<int> GetListMember(const pb::WorldObject& object, const std::string& key, PbObjectIdMap& objectIdMap);
    // element
    static float GetFloatElement(const pb::WorldObject& object, int index, const pb::Prefab& prefab);
    // world
    static Transform* FindTransform(int id);
    // load
    static void LoadFromFile(std::string path);

    PrefabLoader(const pb::Prefab* prefab);
    void Load();

    const pb::Prefab& Prefab() { return *m_Prefab; };
    const pb::WorldObject& GetObject(int id);

private:
    const pb::Prefab* m_Prefab;
    PbObjectIdMap m_ObjectIdMap;

    void BuildObjectIdMap();
    void LoadEntity(const pb::WorldObject& object);

};