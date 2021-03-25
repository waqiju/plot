#include "plot_helper.h"
#include "space_grid_component.h"
#include "core_component/core_component.h"
#include "floating_panel.h"


namespace PlotHelper
{
    std::string kPlotRootName = "PlotRoot";
    std::string kPlotMainPanelName = "PlotMainPanel";
    std::string kPlotFloatingPanelNamePrefix = "PlotFloatingPanel";
    std::string kPlotEssentialTag = "PlotEssential";

    Entity* CreatePlotRootEntity()
    {
        // PlotRoot
        auto plotRootEntity = World::ActiveWorld()->CreateEntity(kPlotRootName);
        plotRootEntity->id = ObjectID::PlotRoot;
        plotRootEntity->tag = kPlotEssentialTag;
        auto plotRootTransform = plotRootEntity->GetComponent<Transform>();
        plotRootTransform->id = ObjectID::PlotRootTransform;
        // PlotMainPanel
        auto plotMainPanelEntity = World::ActiveWorld()->CreateEntity(kPlotMainPanelName);
        plotMainPanelEntity->id = ObjectID::PlotMainPanelRoot;
        plotMainPanelEntity->tag = kPlotEssentialTag;
		plotMainPanelEntity->GetTransform()->SetParent(plotRootTransform);
        // SpaceGrid
        auto spaceGridEntity = World::ActiveWorld()->CreateEntity("SpaceGrid");
        spaceGridEntity->tag = kPlotEssentialTag;
        spaceGridEntity->GetComponent<Transform>()->SetParent(plotMainPanelEntity->GetTransform());
        spaceGridEntity->AddComponent<SpaceGridComponent>();

        return plotRootEntity;
    }

    Entity* CreateFloatingPanelRootEntity(int index)
    {
        auto plotRootTransform = ObjectID::Find(ObjectID::PlotRootTransform)->ToTransform();
        // Entity
        std::string panelName = kPlotFloatingPanelNamePrefix + "_" + std::to_string(index);
        int panelId = ObjectID::PlotFloatingPanelRootStart + index;
        auto panelEntity = World::ActiveWorld()->CreateEntity(panelName);
        panelEntity->id = panelId;
        panelEntity->GetTransform()->SetParent(plotRootTransform);
        // Component
        panelEntity->AddComponent<FloatingPanel>();
        panelEntity->name = panelName;

        return panelEntity;
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

    void CollectBoundsInChildren(Entity* plotEntity, Bounds& fullBounds)
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