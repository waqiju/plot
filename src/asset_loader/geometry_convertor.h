#pragma once
#include "prefab_loader.h"
#include "pb/prefab.pb.h"
#include "geometry/bounds.h"

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