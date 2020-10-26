#include "camera.h"
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "cmath"
#include <iostream>
#include "core_component/core_component.h"
#include "plot/plot.h"


namespace CameraHelper
{
    Camera* CreateCamera()
    {
        Entity* entity = World::ActiveWorld()->CreateEntity();
        Transform* transform = entity->GetComponent<Transform>();
        transform->SetLocalPosition(Vector3(0, 0, -10));
        Camera* camera = entity->AddComponent<Camera>();
        camera->fieldOfView = 45.0f * Mathf::Deg2Rad;
        camera->aspect = (float)Application::screenWidth / Application::screenHeight;
        camera->nearClipPlane = 0.1f;
        camera->farClipPlane = 1000;
        // 设置为主相机
        Application::SetMainCamera(camera);

        return camera;
    }

    void ZoomViewportAxis(Camera* camera, float x, float y)
    {
        // x -> aspect
        if (!Mathf::IsZero(x))
        {
            camera->aspect *= x;
        }
        // y -> fov
        if (!Mathf::IsZero(y))
        {
            float h0 = tan(camera->fieldOfView / 2);
            float h1 = h0 * y;
            camera->fieldOfView = atan(h1) * 2;
            // 如果用 fov 和 aspect 来模拟缩放，改变了 fov 之后，需要对冲下 aspect
            // 可以试试不通过 fov, 通过 farClipPlane 会不会更方便
            camera->aspect /= y;
        }
    }

    void FocusToIntervalX(Camera* camera, float begin, float end)
    {
        Entity* plotEntity = PlotHelper::FindPlotRootEntity();
        if (plotEntity == NULL)
        {
            std::cout << "Not found [PlotRoot] entity!" << std::endl;
            return;
        }

        Bounds fullBounds(Vector3(begin, 1e8f, 0), Vector3(end, -1e8f, 0));
        PlotHelper::CollectPlotRootBounds(plotEntity, fullBounds);
        if (fullBounds.min.y > fullBounds.max.y)
        {
            std::cout << "There is no bounds." << std::endl;
            return;
        }
        else
        {
            fullBounds.Expand(Vector3(1, 1.05f, 0));
        }
        // std::cout << fullBounds.ToString() << std::endl;
        // move focus
        Vector3 previousFrameCenter;
        Ray ray = camera->ViewportPointToRay(Vector3::zero);
        Physics::Raycast(ray, Plane::XyPlane, previousFrameCenter);

        Vector3 currentFrameCenter = fullBounds.Center();
        Vector3 move = currentFrameCenter - previousFrameCenter;
        Transform* cameraTr = camera->GetComponent<Transform>();
        Vector3 currentCameraPosition = cameraTr->LocalPosition() + move;
        cameraTr->SetLocalPosition(currentCameraPosition);
        // fov and aspect
        // 这里是简化过的了，假设相机的 z 轴永远和 xy 平面垂直
        Vector3 worldExtent = fullBounds.Extent();
        camera->fieldOfView = atan(worldExtent.y / (currentCameraPosition - currentFrameCenter).Magnitude()) * 2;
        camera->aspect = worldExtent.x / worldExtent.y;
    }

    void ZoomViewportAndFocusPlot2D(Camera* camera, float x)
    {
        // position
        Vector3 previousFrameCenter, previousFrameMin, previousFrameMax;
        Ray ray = camera->ViewportPointToRay(Vector3::zero);
        Physics::Raycast(ray, Plane::XyPlane, previousFrameCenter);
        ray = camera->ViewportPointToRay(Vector3(-1, -1, 0));
        Physics::Raycast(ray, Plane::XyPlane, previousFrameMin);
        ray = camera->ViewportPointToRay(Vector3(1, 1, 0));
        Physics::Raycast(ray, Plane::XyPlane, previousFrameMax);

        // bounds
        float left = previousFrameCenter.x + (previousFrameMin.x - previousFrameCenter.x) * x;
        float right = previousFrameCenter.x + (previousFrameMax.x - previousFrameCenter.x) * x;
        FocusToIntervalX(camera, left, right);
    }

    // 以相机中心的射线与 xy 平面的交点位置，计算出一像素对应的宽高
    // 用作基准像素宽高
    float OnePixelSizeInWorld(Camera* camera, float screenHeight)
    {
        float d;
        Ray ray = camera->ViewportPointToRay(Vector3::zero);
        Physics::Raycast(ray, Plane::XyPlane, d);
        d = Mathf::Abs(d);

        float h = tan(camera->fieldOfView * 0.5f) * d;
        return h / screenHeight * 2;
    }

    Vector3 OnePixelSizeInLocal(Camera* camera, float screenHeight, const Matrix4x4& localToWorldMatrix)
    {
        float sizeInWorld = OnePixelSizeInWorld(camera, screenHeight);
        Vector3 scaling = localToWorldMatrix.DecomposeScaling();
        return Vector3(sizeInWorld / scaling.x, sizeInWorld / scaling.y, sizeInWorld / scaling.z);
    }
}