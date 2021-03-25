#pragma once
#include "camera.h"
#include "geometry/geometry.h"
#include "entity/ec.h"
#include "core_component/core_component.h"
#include "mesh.h"


class FloatingPanel : public Component, public IBoundsGetter
{
public:
    enum class Kind
    {
        Full,
        Region,
    };

    FloatingPanel(Entity* owner) : Component(owner) {}
    void Update();
    void Render();

    std::string panelName = "Untitled";
    Kind kind = Kind::Full;
    float verticalStart = 0;
    float verticalEnd = 1;

    // Bounds
    Bounds bounds;
    Bounds localBounds;
    Bounds& GetBounds() { return bounds; }

private:
    void RenderTitle();
    void RenderBackground();
    Vector3 GetTitlePosition();

    static Color kBackgroundColor;
};