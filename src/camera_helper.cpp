#include "camera.h"
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include "cmath"


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
        camera->nearClipPlane = 0.1;
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
}