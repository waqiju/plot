#include "gl_headers.h"
#include <iostream>
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "plot/plot.h"
#include "ui_helper.h"
#include "camera_helper.h"
#include "core_component/core_component.h"
#include "common/common.h"
#include "pb/prefab.pb.h"
#include <map>
#include <asset_loader/asset_loader.h>


void OnFrameUpdate();
void LoadPrefab(std::string path);
// void LoadEntity(pb::WorldObject object);

SpaceGridComponent* g_SpaceGrid;
Transform* g_PlotRoot;


int main()
{
    Window* window = Window::CreateWindow("Chimera", Application::screenWidth, Application::screenHeight);
    window->onWindowSizeChanged = UiHelper::WindowSizeChangedHandler;
    window->onMouseScrollChanged = MouseScrollHandlers::ZoomPlotRoot;
    CameraHelper::CreateCamera();

    // PlotRoot
    auto plotRootEntity = World::ActiveWorld()->CreateEntity("PlotRoot");
    g_PlotRoot = plotRootEntity->GetComponent<Transform>();
    // SpaceGrid
    auto entity = World::ActiveWorld()->CreateEntity("SpaceGrid");
    entity->GetComponent<Transform>()->SetParent(g_PlotRoot);
    g_SpaceGrid = entity->AddComponent<SpaceGridComponent>();
    // rectangle
    LoadPrefab("art/triangle.prefab");

    window->FrameLoop(OnFrameUpdate);
    window->Close();
    return 0;
}

void OnFrameUpdate()
{
    UiHelper::ProcessPan();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto usCp : World::ActiveWorld()->GetComponentsInAllEnities<UniformScaleComponent>())
    {
        usCp->ResizeBounds();
    }
    for (auto boundsCp : World::ActiveWorld()->GetComponentsInAllEnities<BoundsComponent>())
    {
        boundsCp->SyncTargetBounds();
    }

    g_SpaceGrid->Render();
    auto rectangleList = World::ActiveWorld()->GetComponentsInAllEnities<chimera::Rectangle>();
    chimera::Rectangle::BatchRender(rectangleList);
    auto triangleList = World::ActiveWorld()->GetComponentsInAllEnities<Triangle>();
    Triangle::BatchRender(triangleList);
    auto pentagramList = World::ActiveWorld()->GetComponentsInAllEnities<Pentagram>();
    Pentagram::BatchRender(pentagramList);
    auto segmentList = World::ActiveWorld()->GetComponentsInAllEnities<chimera::Segment>();
    chimera::Segment::BatchRender(segmentList);
    auto textCpList = World::ActiveWorld()->GetComponentsInAllEnities<TextComponent>();
    TextComponent::BatchRender(textCpList);
}

void LoadPrefab(std::string path)
{
    // one
    auto rectangle = chimera::Rectangle::Create(g_PlotRoot, Vector3(0, 2, 0), Vector3(1, 3, 0), Color::white);
    auto boundsCp = rectangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = rectangle->bounds;
    // two
    std::string content;
    FileHelper::Read(path, content);
    pb::Prefab prefab;
    prefab.ParseFromString(content);

    // load
    auto loader = PrefabLoader(&prefab);
    loader.Load();
}