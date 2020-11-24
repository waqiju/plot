#pragma once
#include "prefab_loader.h"
#include "pb/prefab.pb.h"
#include "geometry/bounds.h"
#include "plot/stock_metadata.h"
#include "graphic/graphic.h"


void ConvertBounds(const pb::WorldObject& object, Bounds& bounds, const pb::Prefab& prefab)
{
    bounds.min = Vector3(
        PrefabLoader::GetFloatElement(object, 0, prefab), 
        PrefabLoader::GetFloatElement(object, 1, prefab),
        PrefabLoader::GetFloatElement(object, 2, prefab));
    bounds.max = Vector3(
        PrefabLoader::GetFloatElement(object, 3, prefab), 
        PrefabLoader::GetFloatElement(object, 4, prefab),
        PrefabLoader::GetFloatElement(object, 5, prefab));
}

void ConvertColor(const pb::WorldObject& object, Color& color, const pb::Prefab& prefab)
{
    color.r = PrefabLoader::GetFloatElement(object, 0, prefab);
    color.g = PrefabLoader::GetFloatElement(object, 1, prefab);
    color.b = PrefabLoader::GetFloatElement(object, 2, prefab);
    color.a = PrefabLoader::GetFloatElement(object, 3, prefab);
}

void ConvertStockMetadata(const pb::WorldObject& object, StockMetadata& metadata, const pb::Prefab& prefab)
{
    metadata.code = PrefabLoader::GetStringMember(object, "code", prefab);
    metadata.data = PrefabLoader::GetStringMember(object, "data", prefab);
    metadata.time = PrefabLoader::GetStringMember(object, "time", prefab);

    metadata.open = PrefabLoader::GetDoubleMember(object, "open", prefab);
    metadata.close = PrefabLoader::GetDoubleMember(object, "close", prefab);
    metadata.low = PrefabLoader::GetDoubleMember(object, "low", prefab);
    metadata.high = PrefabLoader::GetDoubleMember(object, "high", prefab);
    metadata.vol = PrefabLoader::GetDoubleMember(object, "vol", prefab);
    metadata.amount = PrefabLoader::GetDoubleMember(object, "amount", prefab);
}

