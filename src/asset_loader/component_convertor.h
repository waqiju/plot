#pragma once
#include "prefab_loader.h"
#include "pb/prefab.pb.h"
#include "geometry/bounds.h"
#include "plot/plot.h"
#include "core_component/core_component.h"
#include <iostream>
#include "camera_helper.h"
#include "prefab_convertor.h"


void AddComponentToEntity(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader)
{
    WorldObject* worldObject = NULL;

    if (pbComponentObj.type() == "Transform")
    {
        auto transform = entity.GetTransform();
        transform->Deserialize(entity, pbComponentObj, loader);
        worldObject = transform;
    }
    else if (pbComponentObj.type() == "Triangle")
    {
        auto triangle = entity.GetOrAddComponent<Triangle>();
        int boundsObjectID = PrefabLoader::GetIntMember(pbComponentObj, "bounds");
        if (boundsObjectID)
        {
            ConvertBounds(loader.GetObject(boundsObjectID), triangle->bounds, loader.Prefab());
        }
        int colorID = PrefabLoader::GetIntMember(pbComponentObj, "color");
        if (colorID)
        {
			ConvertColor(loader.GetObject(colorID), triangle->color, loader.Prefab());
        }
        triangle->isFlipY = PrefabLoader::GetIntMember(pbComponentObj, "isFlipY");

        worldObject = triangle;
    }
    else if (pbComponentObj.type() == "Pentagram")
    {
        auto pentagram = entity.GetOrAddComponent<Pentagram>();
        int boundsObjectID = PrefabLoader::GetIntMember(pbComponentObj, "bounds");
        if (boundsObjectID)
        {
            ConvertBounds(loader.GetObject(boundsObjectID), pentagram->bounds, loader.Prefab());
        }
        int colorID = PrefabLoader::GetIntMember(pbComponentObj, "color");
        if (colorID)
        {
            ConvertColor(loader.GetObject(colorID), pentagram->color, loader.Prefab());
        }

        worldObject = pentagram;
    }
    else if (pbComponentObj.type() == "Segment")
    {
        auto segment = entity.GetOrAddComponent<chimera::Segment>();
        int boundsObjectID = PrefabLoader::GetIntMember(pbComponentObj, "bounds");
        if (boundsObjectID)
        {
            ConvertBounds(loader.GetObject(boundsObjectID), segment->bounds, loader.Prefab());
        }
        int colorID = PrefabLoader::GetIntMember(pbComponentObj, "color");
        if (colorID)
        {
            ConvertColor(loader.GetObject(colorID), segment->color, loader.Prefab());
        }

        worldObject = segment;
    }
	else if (pbComponentObj.type() == "StockGlyph")
	{
		auto stockGlyph = entity.GetOrAddComponent<PriceDigestGlyph>();
		float x = PrefabLoader::GetFloatMember(pbComponentObj, "x", loader.Prefab());
        //
        StockPriceDigest metadata;
		int metadataID = PrefabLoader::GetIntMember(pbComponentObj, "metadata");
		ConvertStockMetadata(loader.GetObject(metadataID), metadata, loader.Prefab());
        //
		stockGlyph->Reset(x, metadata);

        worldObject = stockGlyph;
	}
    else if (pbComponentObj.type() == "BoundsComponent")
    {
        auto boundsCp = entity.GetOrAddComponent<BoundsComponent>();
        int targetID = PrefabLoader::GetIntMember(pbComponentObj, "target");
        boundsCp->target = PrefabLoader::FindObject<Component>(targetID);

        worldObject = boundsCp;
    }
    else if (pbComponentObj.type() == "UniformScaleComponent")
    {
        auto usCp = entity.GetOrAddComponent<UniformScaleComponent>();
        usCp->mode = static_cast<UniformScaleMode>(PrefabLoader::GetIntMember(pbComponentObj, "mode"));
		int targetID = PrefabLoader::GetIntMember(pbComponentObj, "target");
        usCp->target = PrefabLoader::FindObject<Component>(targetID);
    }
    else if (pbComponentObj.type() == "StockLayoutItem")
    {
        auto layoutItem = entity.GetOrAddComponent<StockLayoutItem>();
        layoutItem->priority = PrefabLoader::GetIntMember(pbComponentObj, "priority");
    }
    else if (pbComponentObj.type() == "StockVerticalLayout")
    {
        auto layout = entity.GetOrAddComponent<StockVerticalLayout>();
        layout->spacing = PrefabLoader::GetIntMember(pbComponentObj, "spacing");
        layout->unit = static_cast<CameraHelper::UnitOfLength>(PrefabLoader::GetIntMember(pbComponentObj, "unit"));
    }
	else if (pbComponentObj.type() == "TextComponent")
	{
		auto textCp = entity.GetOrAddComponent<TextComponent>();
		textCp->Deserialize(entity, pbComponentObj, loader);
        worldObject = textCp;
	}
    else
    {
        std::cout<<"Unsupported component[" << pbComponentObj.type() << "]\n";
    }

    if (worldObject != NULL)
    {
        worldObject->id = pbComponentObj.id();
    }
}