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
void GenerateRandomRectangle();

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
    GenerateShape();
    // GenerateRandomRectangle();

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
        boundsCp->AlignBounds();
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
    // TextComponent::BatchRender(textCpList);
    for (auto textCp:textCpList)
    {
        textCp->AlignBounds();
        textCp->Render();
    }
}

void GenerateShape()
{
    // one
 //    auto rectangle = Rectangle::Create(g_PlotRoot, Vector3(-1, -1, 0), Vector3(1, 1, 0), Color::white);
	// auto boundsCp = rectangle->AddComponent<BoundsComponent>();
	// boundsCp->SetLocalBounds(rectangle->bounds);
 //    // two
 //    rectangle = Rectangle::Create(g_PlotRoot, Vector3(2, -1, 0), Vector3(4, 1, 0), Color::white);
 //    boundsCp = rectangle->AddComponent<BoundsComponent>();
 //    boundsCp->target = rectangle;
 //    auto usCp = rectangle->AddComponent<UniformScaleComponent>();
 //    usCp->mode = UniformScaleMode::FixedWidthInWorldSpace;
 //    usCp->target = rectangle;
 //    boundsCp->SetLocalBounds(rectangle->bounds);
 //    // 三角形
 //    auto triangle = Triangle::Create(g_PlotRoot, Vector3(-4, -1, 0), Vector3(-2, 1, 0), Color::white);
 //    // 五角星
 //    auto pentagram = Pentagram::Create(g_PlotRoot, Vector3(-4, 2, 0), Vector3(-2, 4, 0), Color::white);
    // 文本
    auto textCp = TextComponent::Create(g_PlotRoot, "1", 100, LayoutAlignment::Center);
    textCp->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
}

void GenerateRandomRectangle()
{
    std::vector<float> yList;
    for (int x = 0; x <= 20; ++x)
    {
        float y;
        if (x == 0)
        {
            y = Random::Range(10, 15);
        }
        else
        {
            y = yList[x-1] + Random::Range(-3, 3);
        }
        yList.push_back(y);
        float height = Random::Range(0.1, 5);

        Vector3 left = Vector3(x - 0.45, y - height / 2, 0);
        Vector3 right = Vector3(x + 0.45, y + height / 2, 0);
        Rectangle::Create(g_PlotRoot, left, right, Color::white);
        // segment
        if (x > 0)
        {
           chimera::Segment::Create(g_PlotRoot, Vector3(x-1, yList[x-1], 0), Vector3(x, yList[x], 0), Color::red);
        }
    }
}