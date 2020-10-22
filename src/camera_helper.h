#pragma once
#include "camera.h"

namespace CameraHelper
{
    Camera* CreateCamera();
    void FocusToIntervalX(Camera* camera, float begin, float end);
    void ZoomViewportAxis(Camera* camera, float x, float y);
    void ZoomViewportAndFocusPlot2D(Camera* camera, float x);
}