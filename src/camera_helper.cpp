#include "camera.h"
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"


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
}