#include "plot_helper.h"
#include "space_grid_component.h"
#include "core_component/core_component.h"


namespace PlotHelper
{
    std::string kPlotRootName = "PlotRoot";

    Entity* CreatePlotRootEntity()
    {
        auto plotRootEntity = World::ActiveWorld()->CreateEntity("PlotRoot");
        plotRootEntity->id = ObjectID::PlotRoot;
        auto plotRootTransform = plotRootEntity->GetComponent<Transform>();
        plotRootTransform->id = ObjectID::PlotRootTransform;
        // SpaceGrid
        auto spaceGridEntity = World::ActiveWorld()->CreateEntity("SpaceGrid");
        spaceGridEntity->GetComponent<Transform>()->SetParent(plotRootTransform);
        spaceGridEntity->AddComponent<SpaceGridComponent>();

        return plotRootEntity;
    }

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
        auto rootLocalToWorld = plotEntity->GetTransform()->LocalToWorldMatrix();
        Bounds fullBoundsInWorld = Bounds(rootLocalToWorld.MultiplyPoint3x4(fullBounds.min)
            , rootLocalToWorld.MultiplyPoint3x4(fullBounds.max));

        for (auto boundsComponent: plotEntity->GetComponentsInChildren<BoundsComponent>())
        {
            auto bounds = boundsComponent->WorldBounds();
            auto& min = bounds.min;
            auto& max = bounds.max;

			bool hasIntersection = !(max.x < fullBoundsInWorld.min.x || min.x > fullBoundsInWorld.max.x);
            if (hasIntersection)
            {
                fullBoundsInWorld.min.y = Mathf::Min(fullBoundsInWorld.min.y, min.y);
                fullBoundsInWorld.max.y = Mathf::Max(fullBoundsInWorld.max.y, max.y);
            }
        }

        auto rootWorldToLocal = plotEntity->GetTransform()->WorldToLocalMatrix();
        fullBounds.min = rootWorldToLocal.MultiplyPoint3x4(fullBoundsInWorld.min);
        fullBounds.max = rootWorldToLocal.MultiplyPoint3x4(fullBoundsInWorld.max);
    }

}