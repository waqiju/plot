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
    Rectangle::Create(g_PlotRoot, Vector3::zero, Vector3(1, 1, 0), Color::white);
    Rectangle::Create(g_PlotRoot, Vector3(1, 1, 0), Vector3(2, 2, 0), Color::white);

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
    for (auto rectangle : World::ActiveWorld()->GetComponentsInRootEnities<Rectangle>())
    {
        rectangle->Render();
    }
}
