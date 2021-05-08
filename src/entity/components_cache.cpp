#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <utility>
#include <iostream>
#include "components_cache.h"


float ComponentsCache::kDefaultExpireTime = 10;



ComponentsCacheRecord::ComponentsCacheRecord()
{
    createTime = -1;
}


bool ComponentsCacheRecord::IsExpired(float expireTime)
{
    time_t nowTime = time(NULL);
    float elapsed = time(NULL) - createTime;
    if (elapsed >= expireTime)
    {
        return false;
    }
    int checkCount = std::min(static_cast<int>(list.size()), 5);
    for (int i = 0; i < checkCount; ++i)
    {
       WorldObject* item = list[i];
       if (!item->HasCreated() || item->InDestroy())
       {
           return false;
       }
    }

    return true;
}



void ComponentsCache::Clear()
{
    m_RecordMap.clear();
}