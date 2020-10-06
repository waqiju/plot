#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "plot/space_grid.h"


Camera* CreateCamera();
void OnFrameUpdate();
void WindowSizeChangedHandler(Window* window, int width, int height);

SpaceGrid* g_SpaceGrid;


int main()
{
    Window* window = Window::CreateWindow("Chimera", Application::screenWidth, Application::screenHeight);
    window->onWindowSizeChanged = WindowSizeChangedHandler;
    CreateCamera();
    // render setup
    glEnable(GL_DEPTH_TEST);

    g_SpaceGrid = new SpaceGrid(Application::MainCamera());

    window->FrameLoop(OnFrameUpdate);
    window->Close();
    return 0;
}

void OnFrameUpdate()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_SpaceGrid->Render();
}

Camera* CreateCamera()
{
    Entity* entity = World::ActiveWorld()->CreateEntity();
    Transform* transform = entity->GetComponent<Transform>();
    transform->SetLocalPosition(Vector3(0, 0, -10));
    Camera* camera = entity->AddComponent<Camera>();
    camera->fieldOfView = 45.0f * Mathf::Deg2Rad;
    camera->aspect = (float)Application::screenWidth / Application::screenHeight;
    camera->nearClipPlane = 5;
    camera->farClipPlane = 15;
    // 设置为主相机
    Application::SetMainCamera(camera);

    return camera;
}

void WindowSizeChangedHandler(Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
    for (auto camera : World::ActiveWorld()->GetComponentsInRootEnities<Camera>())
    {
        Application::screenWidth = width;
        Application::screenHeight = height;
        camera->aspect = (float)width / height;
    }
}