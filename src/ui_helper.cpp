#include "ui_helper.h"
#include "entity/ec.h"
#include "application/application.h"
#include "camera.h"
#include <iostream>
#include "geometry/geometry.h"


namespace UiHelper
{
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

    void MouseScrollHandler(Window* window, double xoffset, double yoffset)
    {
        // 鼠标滚轮 zoom
        auto tr = Application::MainCamera()->GetComponent<Transform>();
        tr->SetLocalPosition(tr->LocalPosition() + tr->Forward() * yoffset);
    }

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
        // TODO 用 ScreenPointToRay 实现是更通用的策略，直接 ScreenToViewport 是近截面
        static Vector3 lastWorldPosition;
        if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_1))
        {
            Vector3 mousePosition = Input::MousePosition();
            Ray ray = Application::MainCamera()->ScreenPointToRay(mousePosition);
            Physics::Raycast(ray, Plane::XyPlane, lastWorldPosition);
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
