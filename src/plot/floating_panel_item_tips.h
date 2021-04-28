#pragma once
#include "camera.h"
#include "geometry/geometry.h"
#include "entity/ec.h"
#include "core_component/core_component.h"
#include "mesh.h"


class FloatingPanelItemTips : public Component, public IPrefabConvertor
{
public:
    const static float kDefaultX;

    FloatingPanelItemTips(Entity* owner) : Component(owner) {}


    float x = kDefaultX;
    std::string tips;

    // Prefab
    void Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader) override;
};