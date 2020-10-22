#include "plot_helper.h"
#include "core_component/core_component.h"


namespace PlotHelper
{
    std::string kPlotRootName = "PlotRoot";

    Entity* FindPlotRootEntity()
    {
        auto rootEntities = World::ActiveWorld()->GetRootEntities();
        for (auto entity : rootEntities)
        {
            if (entity->name == kPlotRootName)
            {
                return entity;
            }
        }

        return NULL;
    }

    void CollectPlotRootBounds(Entity* plotEntity, Bounds& fullBounds)
    {
        for (auto boundsComponent: plotEntity->GetComponentsInChildren<BoundsComponent>())
        {
            auto bounds = boundsComponent->WorldBounds();
            auto& min = bounds.min;
            auto& max = bounds.max;

            if ((fullBounds.min.x <= min.x && min.x <= fullBounds.max.x)
                || (fullBounds.min.x <= max.x && max.x <= fullBounds.max.x))
            {
                fullBounds.min.y = Mathf::Min(fullBounds.min.y, min.y);
                fullBounds.max.y = Mathf::Max(fullBounds.max.y, max.y);
            }
        }
    }

}