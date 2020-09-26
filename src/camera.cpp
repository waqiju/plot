#include <assert.h>
#include <glad/glad.h>
#include "geometry/geometry.h"
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
