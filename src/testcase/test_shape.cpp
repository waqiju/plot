#include "gl_headers.h"
#include <iostream>
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "plot/plot.h"
#include "ui_helper.h"
#include "camera_helper.h"
#include "primitive/primitive.h"
using namespace chimera;


void OnFrameUpdate();
void GenerateShapes();

SpaceGrid* g_SpaceGrid;


int main()
{
    Window* window = Window::CreateWindow("Chimera", Application::screenWidth, Application::screenHeight);
    window->onWindowSizeChanged = UiHelper::WindowSizeChangedHandler;
    window->onMouseScrollChanged = MouseScrollHandlers::ChangeFovAndAspect;
    CameraHelper::CreateCamera();

    g_SpaceGrid = new SpaceGrid(Application::MainCamera());
    GenerateShapes();

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
    auto triangles = World::ActiveWorld()->GetComponentsInAllEnities<FlexibleTriangle>();
    for (auto& triangle : triangles)
    {
        triangle->Render();
    }
}

void GenerateShapes()
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto triangle = entity->AddComponent<FlexibleTriangle>();
    triangle->center = Vector3::zero;
    triangle->height = triangle->width = 1;
}