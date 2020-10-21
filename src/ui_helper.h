#pragma once
#include "window.h"

namespace UiHelper
{
    void WindowSizeChangedHandler(Window* window, int width, int height);
    void ProcessPan();
}

namespace MouseScrollHandlers
{
    void MoveCamera(Window* window, double xoffset, double yoffset);
    void ChangeFovAndAspect(Window* window, double xoffset, double yoffset);
    void ZoomPlotRoot(Window* window, double xoffset, double yoffset);
}