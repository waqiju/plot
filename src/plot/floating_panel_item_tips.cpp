#include "floating_panel_item_tips.h"


const float FloatingPanelItemTips::kDefaultX = -1e20f;


void FloatingPanelItemTips::Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader)
{
    assert(pbComponentObj.type() == "FloatingPanelItemTips");

    this->x = PrefabLoader::GetFloatMember(pbComponentObj, "x", loader.Prefab());
    this->tips = PrefabLoader::GetStringMember(pbComponentObj, "tips", loader.Prefab());
}