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
            Application::screenWidth = width;
            Application::screenHeight = height;
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
        Transform* mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
        if (mainPanelTr == NULL)
        {
            std::cout << "Not found [PlotRoot] entity!" << std::endl;
            return;
        }

        Vector3 screenCenterInWorld;
        Ray ray = Application::MainCamera()->ViewportPointToRay(Vector3::zero);
        Physics::Raycast(ray, Plane::XyPlane, screenCenterInWorld);
        Vector3 screenCenterInLocal = mainPanelTr->WorldToLocalMatrix().MultiplyPoint3x4(screenCenterInWorld);
        // plot scale
        mainPanelTr->SetLocalScale(mainPanelTr->LocalScale() / scale);
        // move camera
        Vector3 focusPositionInWorld = mainPanelTr->LocalToWorldMatrix().MultiplyPoint3x4(screenCenterInLocal);
        Vector3 move = focusPositionInWorld - screenCenterInWorld;
        Transform* cameraTr = Application::MainCamera()->GetTransform();
        cameraTr->SetLocalPosition(cameraTr->LocalPosition() + move);

        std::cout << mainPanelTr->LocalScale().ToString() << std::endl;
    }

    void ZoomPlotRootAndFitBounds(Vector3 scale)
    {
        Transform* mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
        if (mainPanelTr == NULL)
        {
            std::cout << "Not found [PlotRoot] entity!" << std::endl;
            return;
        }
        // bounds
        Bounds screenBoundsInWorld = CameraHelper::VisibleAreaInXyPlane(Application::MainCamera());

        Bounds screenBoundsInLocal;
        screenBoundsInLocal.min = mainPanelTr->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.min);
        screenBoundsInLocal.max = mainPanelTr->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.max);
        screenBoundsInLocal.Expand(scale);
        FocusPlot(screenBoundsInLocal.min.x, screenBoundsInLocal.max.x);

        // std::cout << plotRootTr->LocalScale().ToString() << std::endl;
    }

    void FocusPlot(float beginX, float endX)
    {
        Transform* mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
		if (mainPanelTr == NULL)
		{
			std::cout << "Not found [PlotRoot] entity!" << std::endl;
			return;
		}

        Bounds plotBoundsInLocal(Vector3(beginX, 1e8f, 0), Vector3(endX, -1e8f, 0));
        PlotHelper::CollectBoundsInChildren(mainPanelTr->OwnerEntity(), plotBoundsInLocal);
		if (plotBoundsInLocal.Size().y <= 0)
		{
			std::cout << "Not found bounds between beginX[" << beginX  << "] endX[" << endX << "]" << std::endl;
			return;
		}
        // std::cout << plotBoundsInLocal.ToString() << std::endl;
        // 底部预留 FloatingPanel 的空白
        Bounds finalBoundsInLocal = plotBoundsInLocal;
        float maxVerticalEnd = PlotHelper::GetFloatingPanelMaxEnd();
        // 上下预留空白
        {
            Vector3 size = plotBoundsInLocal.Size();
            Vector3 min = plotBoundsInLocal.min;
            Vector3 max = plotBoundsInLocal.max;
            float ratio = size.y / (1 - maxVerticalEnd - 0.1);
            min.y -= ratio * (maxVerticalEnd + 0.05f);
            max.y += ratio * 0.05f;
            finalBoundsInLocal = Bounds(min, max);
        }

        // viewport bounds in world
        Bounds screenBoundsInWorld = CameraHelper::VisibleAreaInXyPlane(Application::MainCamera());;
        // scale
        Vector3 scale = screenBoundsInWorld.Size() / finalBoundsInLocal.Size();
		scale.z = 1;
        // apply scale
        mainPanelTr->SetLocalScale(scale);
        Transform* cameraTr = Application::MainCamera()->GetTransform();

        // move
        Camera* camera = Application::MainCamera();
        Vector3 screenCenterInWorld;
        Ray ray = camera->ViewportPointToRay(Vector3::zero);
        Physics::Raycast(ray, Plane::XyPlane, screenCenterInWorld);
        Vector3 focusPositionInWorld = mainPanelTr->LocalToWorldMatrix().MultiplyPoint3x4(finalBoundsInLocal.Center());
        Vector3 move = focusPositionInWorld - screenCenterInWorld;
        // apply move
        cameraTr->SetLocalPosition(cameraTr->LocalPosition() + move);
    }

}