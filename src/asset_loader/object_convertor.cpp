#pragma once
#include "object_convertor.h"
#include "geometry/geometry.h"
#include "plot/stock_price_digest.h"
#include "graphic/graphic.h"


void ConvertVector3(const pb::WorldObject& object, Vector3& vector, const pb::Prefab& prefab)
{
    vector.x = PrefabLoader::GetFloatElement(object, 0, prefab);
    vector.y = PrefabLoader::GetFloatElement(object, 1, prefab);
    vector.z = PrefabLoader::GetFloatElement(object, 2, prefab);
}

void ConvertColor(const pb::WorldObject& object, Color& color, const pb::Prefab& prefab)
{
    color.r = PrefabLoader::GetFloatElement(object, 0, prefab);
    color.g = PrefabLoader::GetFloatElement(object, 1, prefab);
    color.b = PrefabLoader::GetFloatElement(object, 2, prefab);
    color.a = PrefabLoader::GetFloatElement(object, 3, prefab);
}

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

void ConvertStockMetadata(const pb::WorldObject& object, StockPriceDigest& metadata, const pb::Prefab& prefab)
{
    metadata.code = PrefabLoader::GetStringMember(object, "code", prefab);
    metadata.data = PrefabLoader::GetStringMember(object, "data", prefab);
    metadata.time = PrefabLoader::GetStringMember(object, "time", prefab);

    metadata.open = PrefabLoader::GetDoubleMember(object, "open", prefab);
    metadata.close = PrefabLoader::GetDoubleMember(object, "close", prefab);
    metadata.previous_close = PrefabLoader::GetDoubleMember(object, "previous_close", prefab);
    metadata.low = PrefabLoader::GetDoubleMember(object, "low", prefab);
    metadata.high = PrefabLoader::GetDoubleMember(object, "high", prefab);
    metadata.vol = PrefabLoader::GetDoubleMember(object, "vol", prefab);
    metadata.amount = PrefabLoader::GetDoubleMember(object, "amount", prefab);
}

