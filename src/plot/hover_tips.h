#pragma once
#include "camera.h"
#include "geometry/geometry.h"
#include "entity/ec.h"
#include "core_component/core_component.h"
#include "mesh.h"


class HoverTips : public Component, public IPrefabConvertor
{
public:
    const static float kDefaultX;

    HoverTips(Entity* owner) : Component(owner) {}


    float x = kDefaultX;
    std::string tips;
    int priority = 0;

    // Prefab
    void Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader) override;
};