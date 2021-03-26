#pragma once
#include "entity/ec.h"
#include <string>

namespace PlotHelper
{
    extern std::string kPlotRootName;
    extern std::string kPlotMainPanelName;
    extern std::string kPlotFloatingPanelNamePrefix;
    extern std::string kPlotEssentialTag;

    Entity* CreatePlotRootEntity();
    Entity* CreateFloatingPanelRootEntity(int index);
    Entity* FindPlotRootEntity(); 
    void CollectBoundsInChildren(Entity* plotEntity, Bounds& fullBounds);
    float GetFloatingPanelMaxEnd();
}