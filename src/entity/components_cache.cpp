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
    time_t nowTimeInMs = time(NULL) * 1000;
    float elapsed = (nowTimeInMs - createTime) / 1000;
    if (elapsed >= expireTime)
    {
        // std::cout <<"Expire, " << elapsed << " " << nowTimeInMs << " " << createTime << std::endl;
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