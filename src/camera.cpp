#include <assert.h>
#include <glad/glad.h>
#include "geometry/geometry.h"
#include "camera.h"

Camera::Camera(Entity* owner) : Component(owner)
{
	name = "Camera";

    fieldOfView = 45 * Mathf::Deg2Rad;
    aspect = 1;
    nearClipPlane = 0.1f;
    farClipPlane = 100;
}

Matrix4x4 Camera::ProjectionMatrix()
{
    return Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
}
