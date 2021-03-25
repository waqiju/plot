#pragma once
#include "camera.h"

namespace CameraHelper
{
    enum class UnitOfLength
    {
        World,
        StandardPixelSize,
    };

    Camera* CreateCamera();
    void FocusToIntervalX(Camera* camera, float begin, float end);
    void ZoomViewportAxis(Camera* camera, float x, float y);
    void ZoomViewportAndFocusPlot2D(Camera* camera, float x);

    float OnePixelSizeInWorld(Camera* camera, float screenHeight);
    Vector3 OnePixelSizeInLocal(Camera* camera, float screenHeight, const Matrix4x4& localToWorldMatrix);
    float CalculateLength(float length, UnitOfLength unit);

    Vector3 ViewportToXyPlane(Camera* camera, Vector3 position);
}