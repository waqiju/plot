#include "ui_helper.h"
#include "entity/ec.h"
#include "application/application.h"
#include "camera.h"
#include <iostream>
#include "geometry/geometry.h"
#include "camera_helper.h"
#include "plot/plot.h"


namespace UiHelper
{
    void WindowSizeChangedHandler(Window* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        for (auto camera : World::ActiveWorld()->GetComponentsInRootEnities<Camera>())
        {
            camera->aspect = static_cast<float>(Application::screenWidth) / static_cast<float>(Application::screenHeight);
        }
    }

    // 允许像素扭曲
    /*
    void WindowSizeChangedHandler(Window* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        for (auto camera : World::ActiveWorld()->GetComponentsInRootEnities<Camera>())
        {
            float widthChangeRatio = static_cast<float>(width) / Application::screenWidth;
            float heightChangeRatio = static_cast<float>(height) / Application::screenHeight;

            Application::screenWidth = width;
            Application::screenHeight = height;
            camera->aspect = camera->aspect * widthChangeRatio / heightChangeRatio;
        }
    }
    */

    void ProcessPan()
    {
        // 键盘移动
        Vector3 direction = Vector3::zero;
        if (Input::GetKey(GLFW_KEY_UP))
        {
            direction += Vector3(0, 1, 0);
        }
        if (Input::GetKey(GLFW_KEY_DOWN))
        {
            direction += Vector3(0, -1, 0);
        }
        if (Input::GetKey(GLFW_KEY_LEFT))
        {
            direction += Vector3(-1, 0, 0);
        }
        if (Input::GetKey(GLFW_KEY_RIGHT))
        {
            direction += Vector3(1, 0, 0);
        }
        if (direction != Vector3::zero)
        {
            auto tr = Application::MainCamera()->GetComponent<Transform>();
            tr->SetLocalPosition(tr->LocalPosition() + direction * Time::DeltaTime());
        }
        // 鼠标移动
        static Vector3 lastWorldPosition;
        if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_1))
        {
            Vector3 mousePosition = Input::MousePosition();
            Ray ray = Application::MainCamera()->ScreenPointToRay(mousePosition);
            Physics::Raycast(ray, Plane::XyPlane, lastWorldPosition);
            std::cout << "Mouse Down, " << mousePosition.ToString() << " , " << lastWorldPosition.ToString() << std::endl;
        }
        if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_1))
        {
            Vector3 mousePosition = Input::MousePosition();
            Ray ray = Application::MainCamera()->ScreenPointToRay(mousePosition);
            Vector3 worldPosition;
            Physics::Raycast(ray, Plane::XyPlane, worldPosition);
            Vector3 panWorldDistance = worldPosition - lastWorldPosition;
            Vector3 nowPosition = Application::MainCamera()->GetComponent<Transform>()->LocalPosition();
            Application::MainCamera()->GetComponent<Transform>()->SetLocalPosition(nowPosition - panWorldDistance);
            // Important, must update lastWorldPosition after change camera transform
            ray = Application::MainCamera()->ScreenPointToRay(mousePosition);
            Physics::Raycast(ray, Plane::XyPlane, lastWorldPosition);
        }
    }
}


namespace MouseScrollHandlers
{
    void MoveCamera(Window* window, double xoffset, double yoffset)
    {
        // 鼠标滚轮 zoom
        auto tr = Application::MainCamera()->GetComponent<Transform>();
        tr->SetLocalPosition(tr->LocalPosition() + tr->Forward() * yoffset);
    }

    void ChangeFovAndAspect(Window* window, double xoffset, double yoffset)
    {
        // 鼠标滚轮 zoom
        // 向后滚动，yoffset = -1, 向前滚动 yoffset = 1
        Camera* camera = Application::MainCamera();
        if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
        {
            float factorX = 1 + 0.1 * -yoffset;
            CameraHelper::ZoomViewportAxis(camera, factorX, 0);
        }
        else if (Input::GetKey(GLFW_KEY_LEFT_ALT))
        {
            float factorY = 1 + 0.1 * -yoffset;
            CameraHelper::ZoomViewportAxis(camera, 0, factorY);
        }
        else
        {
            float factorX = 1 + 0.1 * -yoffset;
            CameraHelper::ZoomPlot2D(camera, factorX);
        }
    }

    void ZoomPlotRoot(Window* window, double xoffset, double yoffset)
    {
        Entity* plotEntity = PlotHelper::FindPlotRootEntity();
        if (plotEntity == NULL)
        {
            std::cout << "Not found [PlotRoot] entity!" << std::endl;
            return;
        }
        
        Transform* tr = plotEntity->GetComponent<Transform>();
        Vector3 factor = Vector3::one;
        bool keyControl = Input::GetKey(GLFW_KEY_LEFT_CONTROL);
        bool keyAlt = Input::GetKey(GLFW_KEY_LEFT_ALT);
        if (keyControl)
        {
            factor.x = 1 + 0.1 * -yoffset;
        }
        if (keyAlt)
        {
            factor.y = 1 + 0.1 * -yoffset;
        }
        if (!keyControl && !keyAlt)
        {
            factor.x = 1 + 0.1 * -yoffset;
            // TODO focus
        }
        tr->SetLocalScale(tr->LocalScale() * factor);
        std::cout << tr->LocalScale().ToString() << std::endl;
    }


}