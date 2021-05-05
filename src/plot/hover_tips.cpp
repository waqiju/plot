#include "hover_tips.h"


const float HoverTips::kDefaultX = -1e20f;


void HoverTips::Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader)
{
    assert(pbComponentObj.type() == "HoverTips");

    this->x = PrefabLoader::GetFloatMember(pbComponentObj, "x", loader.Prefab());
    this->tips = PrefabLoader::GetStringMember(pbComponentObj, "tips", loader.Prefab());
    this->priority = PrefabLoader::GetIntMember(pbComponentObj, "priority");
}