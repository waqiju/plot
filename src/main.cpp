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
#include "testcase/test_floating_panel.h"


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
    ChimeraServerImpl::ExecuteFile("art/test_floating_panel.cmd");
    // Server
    ChimeraServerImpl::RunServer();
    // Test
    // TestFloatingPanel_Full();

    window->FrameLoop(OnFrameUpdate);
    window->Close();
    return 0;
}

void OnFrameUpdate()
{
	ChimeraServerImpl::service.ConsumeCommand();
    if (Application::MainWindow()->IsWindowIconic())
    {
        return;
    }

    UiHelper::ProcessPan();
    glClearColor(71.0f/256.0f, 71.0f/256.0f, 71.0f/256.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto panelCp : World::ActiveWorld()->GetComponentsInAllEnities<FloatingPanel>())
    {
        panelCp->Update();
    }
    for (auto usCp : World::ActiveWorld()->GetComponentsInAllEnities<UniformScaleComponent>())
    {
        usCp->ResizeBounds();
    }
    for (auto boundsCp : World::ActiveWorld()->GetComponentsInAllEnities<BoundsComponent>())
    {
        boundsCp->SyncTargetBounds();
    }
    for (auto layoutCp : World::ActiveWorld()->GetComponentsInAllEnities<StockVerticalLayout>())
    {
        layoutCp->ApplyLayout();
    }

    // 网格部分，先绘制线条
    auto spaceGridList = World::ActiveWorld()->GetComponentsInAllEnities<SpaceGridComponent>();
    for (auto spaceGrid : spaceGridList)
        spaceGrid->RenderLine();
    // Panel
    for (auto panelCp : World::ActiveWorld()->GetComponentsInAllEnities<FloatingPanel>())
    {
        panelCp->Render();
    }
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
    auto stockGlyphList = World::ActiveWorld()->GetComponentsInAllEnities<PriceDigestGlyph>();
    PriceDigestGlyph::BatchRender(stockGlyphList);
    // 网格部分，最后绘制 Label
    for (auto spaceGrid : spaceGridList)
        spaceGrid->RenderLabel();
}