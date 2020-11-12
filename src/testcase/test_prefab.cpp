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


int main()
{
    Window* window = Window::CreateWindow("Chimera", Application::screenWidth, Application::screenHeight);
    window->onWindowSizeChanged = UiHelper::WindowSizeChangedHandler;
    window->onMouseScrollChanged = MouseScrollHandlers::ZoomPlotRoot;
    CameraHelper::CreateCamera();

    // PlotRoot
    auto plotRootEntity = PlotHelper::CreatePlotRootEntity();
    // Prefab
    // PrefabLoader::LoadFromFile("art/triangle.prefab");
    PrefabLoader::LoadFromFile("art/stock.prefab");

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

    auto spaceGridList = World::ActiveWorld()->GetComponentsInAllEnities<SpaceGridComponent>();
    for (auto spaceGrid : spaceGridList)
        spaceGrid->Render();

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
}