#pragma once
#include "window.h"

namespace UiHelper
{
    void WindowSizeChangedHandler(Window* window, int width, int height);
    void MouseScrollHandler(Window* window, double xoffset, double yoffset);
    void ProcessPan();
}
