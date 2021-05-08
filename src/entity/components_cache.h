#pragma once
#include <vector>
#include <unordered_map>
#include <ctime>
#include "geometry/random.h"
#include "world_object.h"


class Entity;
class Component;
class WorldObject;


class ComponentsCacheRecord
{

public:
    ComponentsCacheRecord();

    std::vector<WorldObject*> list;
    time_t createTime;

    bool IsExpired(float expireTime = 1);
};


class ComponentsCache
{
public:
    static float kDefaultExpireTime;

    template <typename T> bool Query(size_t typeCode, float expireTime, std::vector<typename T*>& componentList);
    template <typename T> void Update(size_t typeCode, std::vector<typename T*>& componentList);
    void Clear();

private:
    std::unordered_map<size_t, ComponentsCacheRecord> m_RecordMap;

};


template <typename T>
inline bool ComponentsCache::Query(size_t typeCode, float expireTime, std::vector<typename T*>& componentList)
{
    if (m_RecordMap.find(typeCode) == m_RecordMap.end())
    {
        return false;
    }

    ComponentsCacheRecord record = m_RecordMap[typeCode];
    if (!record.IsExpired(expireTime))
    {
        return false;
    }

    componentList.clear();
    // for
    // componentList.reserve(record.list.size());
    // for (WorldObject* item : record.list)
    // {
    //     if (!item->HasCreated() || item->InDestroy())
    //     {
    //        componentList.clear();
    //        return false;
    //     }

    //     componentList.push_back(reinterpret_cast<T*>(item));
    // }
    // memcpy
    componentList.resize(record.list.size());
    memcpy(componentList.data(), record.list.data(), record.list.size() * sizeof(WorldObject*));
    return true;
}


template <typename T>
inline void ComponentsCache::Update(size_t typeCode, std::vector<typename T*>& componentList)
{
    if (m_RecordMap.find(typeCode) == m_RecordMap.end())
    {
        m_RecordMap[typeCode] = ComponentsCacheRecord();
    }

    ComponentsCacheRecord& record = m_RecordMap[typeCode];
    record.createTime = time(NULL) - Random::Range(0, ComponentsCache::kDefaultExpireTime* 0.5f);
    record.list.clear();
    // for
    // record.list.reserve(componentList.size());
    // for (WorldObject* item : componentList)
    // {
    //     record.list.push_back(item);
    // }
    // memcpy
    record.list.resize(componentList.size());
    memcpy(record.list.data(), componentList.data(), componentList.size() * sizeof(WorldObject*));
}