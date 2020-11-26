#pragma once
#include "pb/prefab.pb.h"
#include "prefab_loader.h"
#include "object_convertor.h"
#include <cassert>


class IPrefabConvertor
{
public:
    virtual void Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader) = 0;
};