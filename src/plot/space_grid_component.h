#pragma once
#include "space_grid.h"
#include "entity/ec.h"


class SpaceGridComponent : public Component
{
public:
	SpaceGridComponent(Entity* owner): Component(owner)
    {
        m_SpaceGrid = new SpaceGrid(Application::MainCamera(), this->GetComponent<Transform>());
        // TODO release
    }
    void Render()
    {
        m_SpaceGrid->Render();
    }

private:
    SpaceGrid* m_SpaceGrid;
};