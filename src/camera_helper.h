#pragma once
#include "camera.h"

namespace CameraHelper
{
    Camera* CreateCamera();
    void ZoomViewportAxis(Camera* camera, float x, float y);
}