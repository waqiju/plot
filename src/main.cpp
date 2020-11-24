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
#include "chimera_server.h"


void OnFrameUpdate();


int main()
{
    Window* window = Window::Create("Chimera", Application::screenWidth, Application::screenHeight);
    window->onWindowSizeChanged = UiHelper::WindowSizeChangedHandler;
    window->onMouseScrollChanged = MouseScrollHandlers::ZoomPlotRoot;
    CameraHelper::CreateCamera();

    // PlotRoot
    auto plotRootEntity = PlotHelper::CreatePlotRootEntity();
    // Command
    // ChimeraServerImpl::ExecuteFile("art/600036.cmd");
    // Prefab
    PrefabLoader::LoadFromFile("D:/1_Workspace/aladdin_prophet/data/prefab/plot.prefab");
    // Server
    // ChimeraServerImpl::RunServer();

    window->FrameLoop(OnFrameUpdate);
    window->Close();
    return 0;
}

void OnFrameUpdate()
{
	ChimeraServerImpl::service.ConsumeCommand();

    UiHelper::ProcessPan();
    glClearColor(71.0f/256.0f, 71.0f/256.0f, 71.0f/256.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto usCp : World::ActiveWorld()->GetComponentsInAllEnities<UniformScaleComponent>())
    {
        usCp->ResizeBounds();
    }
    for (auto boundsCp : World::ActiveWorld()->GetComponentsInAllEnities<BoundsComponent>())
    {
        boundsCp->SyncTargetBounds();
    }

    // 网格部分，先绘制线条
    auto spaceGridList = World::ActiveWorld()->GetComponentsInAllEnities<SpaceGridComponent>();
    for (auto spaceGrid : spaceGridList)
        spaceGrid->RenderLine();
    // 基本图形
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
    auto stockGlyphList = World::ActiveWorld()->GetComponentsInAllEnities<StockGlyph>();
    StockGlyph::BatchRender(stockGlyphList);
    // 网格部分，最后绘制 Label
    for (auto spaceGrid : spaceGridList)
        spaceGrid->RenderLabel();
}