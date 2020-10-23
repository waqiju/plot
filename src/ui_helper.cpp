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
            CameraHelper::ZoomViewportAndFocusPlot2D(camera, factorX);
        }
    }

    void ZoomPlotRootAndFocusCenter(Vector3 scale);
    void ZoomPlotRootAndFitBounds(Vector3 scale);

    void ZoomPlotRoot(Window* window, double xoffset, double yoffset)
    {
        Vector3 scale = Vector3::one;
        Vector3 cameraMoveDistance = Vector3::zero;
        bool keyControl = Input::GetKey(GLFW_KEY_LEFT_CONTROL);
        bool keyAlt = Input::GetKey(GLFW_KEY_LEFT_ALT);
        if (keyControl)
        {
            scale.x = 1 - 0.1f * yoffset;
        }
        if (keyAlt)
        {
            scale.y = 1 - 0.1f * yoffset;
        }

        if (keyControl || keyAlt)
        {
            ZoomPlotRootAndFocusCenter(scale);
        }
        else
        {
            scale.x = 1 - 0.1f * yoffset;
            ZoomPlotRootAndFitBounds(scale);
        }

    }

    void ZoomPlotRootAndFocusCenter(Vector3 scale)
    {
        Entity* plotEntity = PlotHelper::FindPlotRootEntity();
        if (plotEntity == NULL)
        {
            std::cout << "Not found [PlotRoot] entity!" << std::endl;
            return;
        }
        Transform* plotRootTr = plotEntity->GetComponent<Transform>();

        Vector3 screenCenterInWorld;
        Ray ray = Application::MainCamera()->ViewportPointToRay(Vector3::zero);
        Physics::Raycast(ray, Plane::XyPlane, screenCenterInWorld);
        Vector3 screenCenterInLocal = plotRootTr->WorldToLocalMatrix().MultiplyPoint3x4(screenCenterInWorld);
        // plot scale
        plotRootTr->SetLocalScale(plotRootTr->LocalScale() / scale);
        // move camera
        Vector3 focusPositionInWorld = plotRootTr->LocalToWorldMatrix().MultiplyPoint3x4(screenCenterInLocal);
        Vector3 move = focusPositionInWorld - screenCenterInWorld;
        Transform* cameraTr = Application::MainCamera()->GetTransform();
        cameraTr->SetLocalPosition(cameraTr->LocalPosition() + move);

        std::cout << plotRootTr->LocalScale().ToString() << std::endl;
    }

    void ZoomPlotRootAndFitBounds(Vector3 scale)
    {
        Entity* plotEntity = PlotHelper::FindPlotRootEntity();
        if (plotEntity == NULL)
        {
            std::cout << "Not found [PlotRoot] entity!" << std::endl;
            return;
        }
        Transform* plotRootTr = plotEntity->GetComponent<Transform>();
        // bounds
        Camera* camera = Application::MainCamera();
        Bounds screenBoundsInWorld;
        Ray ray = camera->ViewportPointToRay(Vector3(-1, -1, 0));
        Physics::Raycast(ray, Plane::XyPlane, screenBoundsInWorld.min);
        ray = camera->ViewportPointToRay(Vector3(1, 1, 0));
        Physics::Raycast(ray, Plane::XyPlane, screenBoundsInWorld.max);

        Bounds screenBoundsInLocal;
        screenBoundsInLocal.min = plotRootTr->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.min);
        screenBoundsInLocal.max = plotRootTr->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.max);
        screenBoundsInLocal.Expand(scale);
        FocusPlot(screenBoundsInLocal.min.x, screenBoundsInLocal.max.x);

        // std::cout << plotRootTr->LocalScale().ToString() << std::endl;
    }

    void FocusPlot(float beginX, float endX)
    {
		Entity* plotEntity = PlotHelper::FindPlotRootEntity();
		if (plotEntity == NULL)
		{
			std::cout << "Not found [PlotRoot] entity!" << std::endl;
			return;
		}

        Bounds plotBoundsInLocal(Vector3(beginX, 0, 0), Vector3(endX, 0, 0));
        PlotHelper::CollectPlotRootBounds(plotEntity, plotBoundsInLocal);
		if (Mathf::IsZero(plotBoundsInLocal.Size().y))
		{
			std::cout << "Not found bounds between beginX[" << beginX  << "] endX[" << endX << "]" << std::endl;
			return;
		}
        // std::cout << plotBoundsInLocal.ToString() << std::endl;

        // viewport bounds in world
        Camera* camera = Application::MainCamera();
        Bounds screenBoundsInWorld;
        Ray ray = camera->ViewportPointToRay(Vector3(-1, -1, 0));
        Physics::Raycast(ray, Plane::XyPlane, screenBoundsInWorld.min);
        ray = camera->ViewportPointToRay(Vector3(1, 1, 0));
        Physics::Raycast(ray, Plane::XyPlane, screenBoundsInWorld.max);

		Transform* plotRootTr = plotEntity->GetTransform();
        // scale
        Vector3 scale = screenBoundsInWorld.Size() / plotBoundsInLocal.Size();
		scale.z = 1;

        // move
        Vector3 screenCenterInWorld;
        ray = camera->ViewportPointToRay(Vector3::zero);
        Physics::Raycast(ray, Plane::XyPlane, screenCenterInWorld);
        Vector3 focusPositionInWorld = plotRootTr->LocalToWorldMatrix().MultiplyPoint3x4(plotBoundsInLocal.Center());
        Vector3 move = focusPositionInWorld - screenCenterInWorld;

        // apply
		plotRootTr->SetLocalScale(scale);
        Transform* cameraTr = Application::MainCamera()->GetTransform();
        cameraTr->SetLocalPosition(cameraTr->LocalPosition() + move);
    }

}