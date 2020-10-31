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
    /*
        stock_glyph
            |- triangle_1
            |- pentagram_2
                |- text_2_1
            |- triangle_3
            |- pentagram_4
    */

	// stock glyph
	KChart chart;
	chart.set_open(0);
	chart.set_close(2);
	chart.set_high(2);
	chart.set_low(0);
	auto stockGlyph = StockGlyph::Create(g_PlotRoot, 0, chart);
	auto layout = stockGlyph->AddComponent<StockVerticalLayout>();
    layout->spacing = 2;
    layout->unit = CameraHelper::UnitOfLength::StandardPixelSize;
    // 1- triangle
	auto vertexPairX = StockGlyph::MeshVertexPairX(0);
	auto triangle1 = Triangle::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::green, true);
    auto boundsCp1 = triangle1->AddComponent<BoundsComponent>();
    boundsCp1->localBounds = triangle1->bounds;
    boundsCp1->target = triangle1;
    auto usCp1 = triangle1->AddComponent<UniformScaleComponent>();
    usCp1->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp1->target = triangle1;
	auto layoutItem1 = triangle1->AddComponent<StockLayoutItem>();
    layoutItem1->priority = 20;
    // 2- pentagram
    auto pentagram2 = Pentagram::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::red);
    auto boundsCp2 = pentagram2->AddComponent<BoundsComponent>();
    boundsCp2->localBounds = pentagram2->bounds;
    boundsCp2->target = pentagram2;
    auto usCp2 = pentagram2->AddComponent<UniformScaleComponent>();
    usCp2->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp2->target = pentagram2;
    auto layoutItem2 = pentagram2->AddComponent<StockLayoutItem>();
    layoutItem2->priority = 10;
    // 2_1 text
    auto textCp2_1 = TextComponent::Create(pentagram2->GetTransform(), "text", 18, LayoutAlignment::Center);
    textCp2_1->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
    auto boundsCp2_1 = textCp2_1->AddComponent<BoundsComponent>();
    boundsCp2_1->localBounds = textCp2_1->bounds;
    boundsCp2_1->target = textCp2_1;
    auto layoutItem2_1 = textCp2_1->AddComponent<StockLayoutItem>();
    layoutItem2_1->priority = 11;
    // 3- triangle
    auto triangle3 = Triangle::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::white);
    auto boundsCp3 = triangle3->AddComponent<BoundsComponent>();
    boundsCp3->localBounds = triangle3->bounds;
    boundsCp3->target = triangle3;
    auto usCp3 = boundsCp3->AddComponent<UniformScaleComponent>();
    usCp3->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp3->target = triangle3;
    auto layoutItem3 = triangle3->AddComponent<StockLayoutItem>();
    layoutItem3->priority = -20;
    // 4- pentagram
    auto pentagram4 = Pentagram::Create(stockGlyph->GetTransform(), Vector3(vertexPairX.first, -1, 0), Vector3(vertexPairX.second, 1, 0), Color::white);
    auto boundsCp4 = pentagram4->AddComponent<BoundsComponent>();
    boundsCp4->localBounds = pentagram4->bounds;
    boundsCp4->target = pentagram4;
    auto layoutItem4 = pentagram4->AddComponent<StockLayoutItem>();
    auto usCp4 = pentagram4->AddComponent<UniformScaleComponent>();
    usCp4->mode = UniformScaleMode::FixedWidthInWorldSpace;
    usCp4->target = pentagram4;
    layoutItem4->priority = -10;
}
