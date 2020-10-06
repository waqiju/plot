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

Vector3 Camera::ScreenToViewportPoint(Vector3 position)
{
    Vector3 result;
    result.x = position.x / Application::screenWidth * 2 - 1;
    result.y = position.y / Application::screenHeight * 2 - 1;
    // OpenGL z �ķ�Χ [0, 1]
    result.z = position.z * 2 - 1;
    return result;
}

Vector3 Camera::ViewportToWorldPoint(Vector3 position)
{
    Matrix4x4 matrix = (ProjectionMatrix() * WorldToCameraMatrix()).Inverse();
    Vector3 result = matrix.MultiplyPoint(position);
    return result;
}

// ���ص��ǽ�����ĵ�
Vector3 Camera::ScreenToWorldPoint(Vector3 position)
{
    Vector3 result = ScreenToViewportPoint(position);
    result = ViewportToWorldPoint(result);
    return result;
}
