#pragma once
#include "entity/ec.h"
#include <string>

namespace PlotHelper
{
    extern std::string kPlotRootName;

    Entity* CreatePlotRootEntity();
    Entity* FindPlotRootEntity(); 
    void CollectPlotRootBounds(Entity* plotEntity, Bounds& fullBounds);
}