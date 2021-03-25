#pragma once
#include "test_floating_panel.h"
#include "plot/plot.h"

#include "command/command.h"
#include <iostream>
using namespace chimera;


void CreateItemInMainPanel();
void CreateItemInFloatingPanel();


void TestFloatingPanel()
{
    // create panel
    auto panelEntity = PlotHelper::CreateFloatingPanelRootEntity(1);
    auto panelCp = panelEntity->GetComponent<FloatingPanel>();
    panelCp->kind = FloatingPanel::Kind::Region;
    panelCp->verticalStart = 0.05f;
    panelCp->verticalEnd = 0.2f;
    // create item
    CreateItemInMainPanel();
    CreateItemInFloatingPanel();
    // tree
    auto content = TreeCommand().Execute();
    std::cout << content << std::endl;
}


void CreateItemInMainPanel()
{
    auto mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
    // 1
    auto rectangle = Rectangle::Create(mainPanelTr, Vector3(0, 0, 0), Vector3(1, 1, 0), Color(1, 0, 0, 1));
    auto boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    // 2
    rectangle = Rectangle::Create(mainPanelTr, Vector3(2, 0, 0), Vector3(3, 2, 0), Color(1, 0, 0, 1));
    boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    // 3
    rectangle = Rectangle::Create(mainPanelTr, Vector3(4, 0, 0), Vector3(5, 3, 0), Color(1, 0, 0, 1));
    boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
}

void CreateItemInFloatingPanel()
{
    auto panelTr = ObjectID::Find(ObjectID::PlotFloatingPanelRootStart + 1)->ToTransform();
    // 1
    auto rectangle = Rectangle::Create(panelTr, Vector3(1, 0, 0), Vector3(2, 2, 0), Color(0, 1, 0, 1));
    auto boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
}