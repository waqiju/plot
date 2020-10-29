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
            auto bounds = boundsComponent->localBounds;
            auto& min = bounds.min;
            auto& max = bounds.max;

			bool hasIntersection = !(max.x < fullBounds.min.x || min.x > fullBounds.max.x);
            if (hasIntersection)
            {
                fullBounds.min.y = Mathf::Min(fullBounds.min.y, min.y);
                fullBounds.max.y = Mathf::Max(fullBounds.max.y, max.y);
            }
        }
    }

}