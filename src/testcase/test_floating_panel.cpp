﻿#include "test_floating_panel.h"
#include "plot/plot.h"
#include "command/command.h"
#include <iostream>
using namespace chimera;


void CreateItemInMainPanel();
void CreateItemInFloatingPanel1();
void CreateItemInFloatingPanel2();
void CreateItemInFloatingPanel3();


void TestFloatingPanel_TwoRegion()
{
    // create item
    CreateItemInMainPanel();
    CreateItemInFloatingPanel1();
    CreateItemInFloatingPanel2();
    // tree
    auto content = TreeCommand().Execute();
    std::cout << content << std::endl;
}


void TestFloatingPanel_Full()
{
    // create item
    CreateItemInMainPanel();
    CreateItemInFloatingPanel1();
    CreateItemInFloatingPanel3();
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
    auto tipsCp = rectangle->AddComponent<HoverTips>();
    tipsCp->x = 0.5f;
    tipsCp->tips = "HoverTips-1";
    // 2
    rectangle = Rectangle::Create(mainPanelTr, Vector3(2, 0, 0), Vector3(3, 2, 0), Color(1, 0, 0, 1));
    boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    tipsCp = rectangle->AddComponent<HoverTips>();
    tipsCp->x = 2.5f;
    tipsCp->tips = "HoverTips-2";
    // 3
    rectangle = Rectangle::Create(mainPanelTr, Vector3(4, 0, 0), Vector3(5, 3, 0), Color(1, 0, 0, 1));
    boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    tipsCp = rectangle->AddComponent<HoverTips>();
    tipsCp->x = 4.5f;
    tipsCp->tips = "HoverTips:\nI am No.3 tips.";
}


void CreateItemInFloatingPanel1()
{
    // create panel
    auto panelEntity = PlotHelper::CreateFloatingPanelRootEntity(1);
    auto panelCp = panelEntity->GetComponent<FloatingPanel>();
    panelCp->kind = FloatingPanel::Kind::Region;
    panelCp->verticalStart = 0.05f;
    panelCp->verticalEnd = 0.2f;
    // create item
    auto panelTr = panelEntity->GetTransform();
    // 1
    auto rectangle = Rectangle::Create(panelTr, Vector3(1, 0, 0), Vector3(2, 2, 0), Color(0, 1, 0, 1));
    auto boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    // 2
    rectangle = Rectangle::Create(panelTr, Vector3(3, 0, 0), Vector3(4, 3, 0), Color(0, 1, 0, 1));
    boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
}


void CreateItemInFloatingPanel2()
{
    // create panel
    auto panelEntity = PlotHelper::CreateFloatingPanelRootEntity(2);
    auto panelCp = panelEntity->GetComponent<FloatingPanel>();
    panelCp->kind = FloatingPanel::Kind::Region;
    panelCp->verticalStart = 0.25f;
    panelCp->verticalEnd = 0.4f;
    // create item
    auto panelTr = panelEntity->GetTransform();
    // 1
    auto rectangle = Rectangle::Create(panelTr, Vector3(1, 0, 0), Vector3(2, 3, 0), Color(0, 1, 0, 1));
    auto boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    auto tipsCp = rectangle->AddComponent<FloatingPanelItemTips>();
    tipsCp->x = 1.5f;
    tipsCp->tips = "3";
    // 2
    rectangle = Rectangle::Create(panelTr, Vector3(3, 0, 0), Vector3(4, 2, 0), Color(0, 1, 0, 1));
    boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    tipsCp = rectangle->AddComponent<FloatingPanelItemTips>();
    tipsCp->x = 3.5f;
    tipsCp->tips = "2";
}


void CreateItemInFloatingPanel3()
{
    // create panel
    auto panelEntity = PlotHelper::CreateFloatingPanelRootEntity(2);
    auto panelCp = panelEntity->GetComponent<FloatingPanel>();
    panelCp->kind = FloatingPanel::Kind::OverlayMainPanel;
    // create item
    auto panelTr = panelEntity->GetTransform();
    // 1
    auto rectangle = Rectangle::Create(panelTr, Vector3(6, 0, 0), Vector3(7, 3, 0), Color(0, 1, 0, 1));
    auto boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    // 2
    rectangle = Rectangle::Create(panelTr, Vector3(8, 0, 0), Vector3(9, 2, 0), Color(0, 1, 0, 1));
    boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
}