#include <assert.h>
#include <glad/glad.h>
#include "geometry/geometry.h"
#include "application/application.h"
#include "entity/ec.h"
#include "camera.h"

Camera::Camera(Entity* owner) : Component(owner)
{
	name = "Camera";

    fieldOfView = 45 * Mathf::Deg2Rad;
    aspect = 1;
    nearClipPlane = 0.1f;
    farClipPlane = 100;
}

Matrix4x4 Camera::WorldToCameraMatrix()
{
    Transform* transform = this->GetComponent<Transform>();
    // TODO use world position instead of local
    Vector3 t = -transform->LocalPosition();
    Quaternion r = transform->LocalRotation().Inverse();
    return Matrix4x4::Rotate(r) * Matrix4x4::Translate(t);
}

Matrix4x4 Camera::ProjectionMatrix()
{
    return Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
}

Matrix4x4 Camera::ViewProjectMatrix()
{
    return ProjectionMatrix() * WorldToCameraMatrix();
}

Vector3 Camera::ScreenToViewportPoint(Vector3 position)
{
    Vector3 result;
    result.x = position.x / Application::screenWidth * 2 - 1;
    // 窗口的左上角是原点
    result.y = -(position.y / Application::screenHeight * 2 - 1);
    // OpenGL z 的范围 [0, 1]
    result.z = position.z * 2 - 1;
    return result;
}

Vector3 Camera::ViewportToScreenPoint(Vector3 position)
{
    Vector3 result;
    result.x = (position.x + 1) * 0.5f * Application::screenWidth;
    // 窗口的左上角是原点
    result.y = (-position.y + 1) * 0.5f * Application::screenHeight;
    // OpenGL z 的范围 [0, 1]
    result.z = (position.z + 1) * 0.5f;
    return result;
}

Ray Camera::ViewportPointToRay(Vector3 pos)
{
    Vector3 cameraPos = this->GetComponent<Transform>()->Position();
    Vector3 sendPos = ViewportToWorldPoint(pos);
    Vector3 direction = Vector3::Normalize(sendPos - cameraPos);

    return Ray(cameraPos, direction);
}

Ray Camera::ScreenPointToRay(Vector3 pos)
{
    Vector3 cameraPos = this->GetComponent<Transform>()->Position();
    Vector3 sendPos = ScreenToWorldPoint(pos);
    Vector3 direction = Vector3::Normalize(sendPos - cameraPos);

    return Ray(cameraPos, direction);
}

Vector3 Camera::ViewportToWorldPoint(Vector3 position)
{
    Matrix4x4 matrix = (ProjectionMatrix() * WorldToCameraMatrix()).Inverse();
    Vector3 result = matrix.MultiplyPoint(position);
    return result;
}

// 返回的是近截面的点
Vector3 Camera::ScreenToWorldPoint(Vector3 position)
{
    Vector3 result = ScreenToViewportPoint(position);
    result = ViewportToWorldPoint(result);
    return result;
}
