#pragma once
#include "camera.h"
#include "geometry/geometry.h"
#include "entity/ec.h"
#include "core_component/core_component.h"
#include "mesh.h"


class FloatingPanel : public Component, public IBoundsGetter, public IPrefabConvertor
{
public:
    enum class Kind
    {
        OverlayMainPanel,
        Region,
    };

    FloatingPanel(Entity* owner) : Component(owner) {}
    void Update();
    void Render();

    std::string panelName = "Untitled";
    Kind kind = Kind::OverlayMainPanel;
    float verticalStart = 0;
    float verticalEnd = 1;

    // Bounds
    Bounds bounds;
    Bounds localBounds;
    Bounds& GetBounds() { return bounds; }

    // Prefab
    void Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader) override;

private:
    std::string m_Title;

    void RenderTitle();
    void RenderBackground();
    Vector3 GetTitlePosition();
    void UpdateTitle();

    static Color kBackgroundColor;
};