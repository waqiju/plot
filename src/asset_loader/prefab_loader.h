#pragma once
#include "pb/prefab.pb.h"


class PrefabLoader
{
public:
    PrefabLoader(pb::Prefab* prefab);

private:
    pb::Prefab* m_Prefab;
}