#include "gl_headers.h"
#include <iostream>
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "plot/plot.h"
#include "ui_helper.h"
#include "camera_helper.h"
#include "pb/prophet.pb.h"
using namespace chimera;


void OnFrameUpdate();
void GenerateRandomStockGlyph();

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

    GenerateRandomStockGlyph();
    // CameraHelper::FocusToIntervalX(Application::MainCamera(), 160, 220);

    window->FrameLoop(OnFrameUpdate);
    window->Close();
    return 0;
}

void OnFrameUpdate()
{
    UiHelper::ProcessPan();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_SpaceGrid->Render();
    auto sgList = g_PlotRoot->GetComponentsInChildren<StockGlyph>();
    StockGlyph::BatchRender(sgList);
}

void GenerateRandomStockGlyph()
{
    std::vector<KChart> chartList;
    for (int x = 0; x <= 200; ++x)
    {
        KChart chart;
        float close;
        if (x == 0)
        {
            close = Random::Range(10, 15);
        }
        else
        {
            close = chartList[x-1].close() + Random::Range(-2.f, 2.2f);
        }
        float open = close * Random::Range(0.97, 1.03) + Random::Range(-1, 1);
        float max = Mathf::Max(open, close);
        float min = Mathf::Min(open, close);
        float high = max * Random::Range(1.001, 1.02);
        float low = min * Random::Range(0.999, 0.98);
        chart.set_open(open);
        chart.set_close(close);
        chart.set_high(high);
        chart.set_low(low);
        chartList.push_back(chart);

        StockGlyph::Create(g_PlotRoot, x, chart);
    }
}