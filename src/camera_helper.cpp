#include "camera.h"
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "cmath"
#include <iostream>
#include "core_component/core_component.h"


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

    Entity* FindPlotRootEntity()
    {
        auto rootEntities = World::ActiveWorld()->GetRootEntities();
        for (auto entity : rootEntities)
        {
            if (entity->name == "PlotRoot")
            {
                return entity;
            }
        }

        return NULL;
    }

    void CollectPlotRootBounds(Entity* plotEntity, Bounds& fullBounds)
    {
        for (auto boundsComponent: plotEntity->GetComponentsInChildren<BoundsComponent>())
        {
            auto& min = boundsComponent->min;
            auto& max = boundsComponent->max;

            if ((fullBounds.min.x <= min.x && min.x <= fullBounds.max.x)
                || (fullBounds.min.x <= max.x && max.x <= fullBounds.max.x))
            {
                fullBounds.min.y = Mathf::Min(fullBounds.min.y, min.y);
                fullBounds.max.y = Mathf::Max(fullBounds.max.y, max.y);
            }
        }
    }

    void ZoomPlot2D(Camera* camera, float x)
    {
        Entity* plotEntity = FindPlotRootEntity();
        if (plotEntity == NULL)
        {
            std::cout << "Not found [PlotRoot] entity!" << std::endl;
            return;
        }

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
        Bounds fullBounds(Vector3(left, 1e8f, 0), Vector3(right, -1e8f, 0));
        CollectPlotRootBounds(plotEntity, fullBounds);
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
}