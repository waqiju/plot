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


void OnFrameUpdate();
void GenerateShape();

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
    // shape
    GenerateShape();

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
	for (auto layoutCp : World::ActiveWorld()->GetComponentsInAllEnities<StockVerticalLayout>())
	{
		layoutCp->ApplyLayout();
	}

    g_SpaceGrid->Render();
    auto rectangleList = World::ActiveWorld()->GetComponentsInAllEnities<Rectangle>();
    Rectangle::BatchRender(rectangleList);
    auto triangleList = World::ActiveWorld()->GetComponentsInAllEnities<Triangle>();
    Triangle::BatchRender(triangleList);
    auto pentagramList = World::ActiveWorld()->GetComponentsInAllEnities<Pentagram>();
    Pentagram::BatchRender(pentagramList);
    auto segmentList = World::ActiveWorld()->GetComponentsInAllEnities<chimera::Segment>();
    chimera::Segment::BatchRender(segmentList);
    auto textCpList = World::ActiveWorld()->GetComponentsInAllEnities<TextComponent>();
    TextComponent::BatchRender(textCpList);
	auto sgList = g_PlotRoot->GetComponentsInChildren<StockGlyph>();
	StockGlyph::BatchRender(sgList);
}

void GenerateShape()
{
	// stock glyph
	KChart chart;
	chart.set_open(0);
	chart.set_close(2);
	chart.set_high(2);
	chart.set_low(0);
	auto stockGlyph = StockGlyph::Create(g_PlotRoot, 0, chart);
	stockGlyph->AddComponent<StockVerticalLayout>();
    // triangle
	auto vertexPairX = StockGlyph::MeshVertexPairX(0);
	auto triangle = Triangle::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::white);
    auto boundsCp = triangle->AddComponent<BoundsComponent>();
    boundsCp->localBounds = triangle->bounds;
    boundsCp->target = triangle;
    auto usCp = triangle->AddComponent<UniformScaleComponent>();
    usCp->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp->target = triangle;
	triangle->AddComponent<StockLayoutItem>();
 //    // one
 //    auto rectangle = Rectangle::Create(g_PlotRoot, Vector3(-1, -1, 0), Vector3(1, 1, 0), Color::white);
	// auto boundsCp = rectangle->AddComponent<BoundsComponent>();
	// boundsCp->localBounds = rectangle->bounds;
 //    // two
 //    rectangle = Rectangle::Create(g_PlotRoot, Vector3(2, -1, 0), Vector3(4, 1, 0), Color::white);
 //    boundsCp = rectangle->AddComponent<BoundsComponent>();
 //    boundsCp->target = rectangle;
 //    auto usCp = rectangle->AddComponent<UniformScaleComponent>();
 //    usCp->mode = UniformScaleMode::FixedWidthInWorldSpace;
 //    usCp->target = rectangle;
 //    boundsCp->localBounds = rectangle->bounds;
 //    // 三角形
 //    auto triangle = Triangle::Create(g_PlotRoot, Vector3(-4, -1, 0), Vector3(-2, 1, 0), Color::white);
 //    // 五角星
 //    auto pentagram = Pentagram::Create(g_PlotRoot, Vector3(-4, 2, 0), Vector3(-2, 4, 0), Color::white);
 //    // 文本
 //    auto textCp = TextComponent::Create(g_PlotRoot, "123456", 100, LayoutAlignment::Center);
 //    textCp->GetTransform()->SetLocalPosition(Vector3(0, -4, 0));
}
