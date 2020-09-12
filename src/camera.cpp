#include <assert.h>
#include <glad/glad.h>
#include "camera.h"

Camera::Camera(Entity* owner) : Component(owner)
{
	name = "Camera";

    fieldOfView = 45;
    aspect = 1;
    farClipPlane = 0.1f;
    nearClipPlane = 100;
}

Matrix4x4 Camera::ProjectionMatrix()
{
    return Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
}
