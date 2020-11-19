#pragma once
#include "space_grid.h"
#include "entity/ec.h"
#include "application/application.h"


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
    void RenderLine()
    {
        m_SpaceGrid->RenderLine();
    }
    void RenderLabel()
    {
        m_SpaceGrid->RenderLabel();
    }

private:
    SpaceGrid* m_SpaceGrid;
};