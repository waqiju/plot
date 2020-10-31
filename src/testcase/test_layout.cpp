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
	auto layout = stockGlyph->AddComponent<StockVerticalLayout>();
    layout->unit = CameraHelper::UnitOfLength::StandardPixelSize;
    return;
    // 1- triangle
	auto vertexPairX = StockGlyph::MeshVertexPairX(0);
	auto triangle1 = Triangle::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::white);
    auto boundsCp1 = triangle1->AddComponent<BoundsComponent>();
    boundsCp1->localBounds = triangle1->bounds;
    boundsCp1->target = triangle1;
    auto usCp1 = triangle1->AddComponent<UniformScaleComponent>();
    usCp1->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp1->target = triangle1;
	triangle1->AddComponent<StockLayoutItem>();
    // 2- triangle
    auto triangle2 = Triangle::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::white);
    auto boundsCp2 = triangle2->AddComponent<BoundsComponent>();
    boundsCp2->localBounds = triangle2->bounds;
    boundsCp2->target = triangle2;
    auto usCp2 = triangle2->AddComponent<UniformScaleComponent>();
    usCp2->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp2->target = triangle2;
    triangle2->AddComponent<StockLayoutItem>();
    // 3- triangle
    auto triangle3 = Triangle::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::white);
    auto boundsCp3 = triangle3->AddComponent<BoundsComponent>();
    boundsCp3->localBounds = triangle3->bounds;
    boundsCp3->target = triangle3;
    auto usCp3 = boundsCp3->AddComponent<UniformScaleComponent>();
    usCp3->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp3->target = triangle3;
    auto layoutItem3 = triangle3->AddComponent<StockLayoutItem>();
    layoutItem3->priority = -1;
    // 4- pentagram
    auto pentagram = Pentagram::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::white);
    auto boundsCp4 = pentagram->AddComponent<BoundsComponent>();
    boundsCp4->localBounds = pentagram->bounds;
    boundsCp4->target = pentagram;
    auto layoutItem4 = pentagram->AddComponent<StockLayoutItem>();
    auto usCp4 = pentagram->AddComponent<UniformScaleComponent>();
    usCp4->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp4->target = pentagram;
    layoutItem4->priority = -1;

 //    // 文本
 //    auto textCp = TextComponent::Create(g_PlotRoot, "123456", 100, LayoutAlignment::Center);
 //    textCp->GetTransform()->SetLocalPosition(Vector3(0, -4, 0));
}
