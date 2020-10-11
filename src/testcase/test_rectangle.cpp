#include "gl_headers.h"
#include <iostream>
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "plot/plot.h"
#include "ui_helper.h"
#include "camera_helper.h"


void OnFrameUpdate();
void GenerateRandomRectangle();

SpaceGrid* g_SpaceGrid;
Transform* g_PlotRoot;


int main()
{
    Window* window = Window::CreateWindow("Chimera", Application::screenWidth, Application::screenHeight);
    window->onWindowSizeChanged = UiHelper::WindowSizeChangedHandler;
    window->onMouseScrollChanged = UiHelper::MouseScrollHandler;
    CameraHelper::CreateCamera();

    g_SpaceGrid = new SpaceGrid(Application::MainCamera());
    // rectangle
    auto plotRootEntity = World::ActiveWorld()->CreateEntity();
    plotRootEntity->name = "PlotRoot";
    g_PlotRoot = plotRootEntity->GetComponent<Transform>();
    GenerateRandomRectangle();

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
    auto rectangleList = World::ActiveWorld()->GetComponentsInRootEnities<Rectangle>();
    Rectangle::BatchRender(rectangleList);
    auto segmentList = World::ActiveWorld()->GetComponentsInRootEnities<Segment>();
    Segment::BatchRender(segmentList);
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
            Segment::Create(g_PlotRoot, Vector3(x-1, yList[x-1], 0), Vector3(x, yList[x], 0), Color::red);
        }
    }
}