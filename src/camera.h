#pragma once
#include "mesh.h"
#include "material.h"
#include "entity/component.h"
#include "geometry/geometry.h"


class Camera : public Component
{
public:
    Camera(Entity* owner);
    Matrix4x4 WorldToCameraMatrix();
    Matrix4x4 ProjectionMatrix();

    float fieldOfView;
    float aspect;
    float farClipPlane;
    float nearClipPlane;
};