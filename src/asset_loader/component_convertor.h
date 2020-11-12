#pragma once
#include "prefab_loader.h"
#include "pb/prefab.pb.h"
#include "geometry/bounds.h"
#include "plot/plot.h"
#include <iostream>


void AddComponentToEntity(Entity& entity, const pb::WorldObject& componentObj, PrefabLoader& loader)
{
    if (componentObj.type() == "Transform")
    {
        auto transform = entity.GetTransform();
        auto parentId = PrefabLoader::GetIntMember(componentObj, "parent");
        transform->SetParent(PrefabLoader::FindTransform(parentId));
    }
    else if (componentObj.type() == "Triangle")
    {
        auto triangle = entity.GetOrAddComponent<Triangle>();
        int boundsObjectID = PrefabLoader::GetIntMember(componentObj, "bounds");
        if (boundsObjectID)
        {
            ConvertBounds(loader.GetObject(boundsObjectID), triangle->bounds, loader.Prefab());
        }
    }
	else if (componentObj.type() == "StockGlyph")
	{
		auto stockGlyph = entity.GetOrAddComponent<StockGlyph>();
		float x = PrefabLoader::GetFloatMember(componentObj, "x", loader.Prefab());
        //
        StockMetadata metadata;
		int metadataID = PrefabLoader::GetIntMember(componentObj, "metadata");
		ConvertStockMetadata(loader.GetObject(metadataID), metadata, loader.Prefab());
        //
		stockGlyph->Reset(x, metadata);
	}
    else
    {
        std::cout<<"Unsupported component[" << componentObj.type() << "]\n";
    }
}