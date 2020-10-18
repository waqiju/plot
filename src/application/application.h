#pragma once
#include <string>
#include <application/input.h>
#include <application/time.h>
#include "geometry/geometry.h"


class Window;
class Camera;


class Application
{
public:
    static std::string ProjectPath;
    // 屏幕宽高
    static const unsigned int kDefaultScreenWidth = 1200;
    static const unsigned int kDefaultScreenHeight = 800;
    static unsigned int screenWidth;
    static unsigned int screenHeight;
    static Vector2 ScreenSize() { return Vector2( static_cast<float>(screenWidth), static_cast<float>(screenHeight)); }

    static Window* MainWindow() { return m_MainWindow; }
    static void SetMainWindow(Window* window) { m_MainWindow = window; }
    static Camera* MainCamera() { return m_MainCamera; }
    static void SetMainCamera(Camera* camera) { m_MainCamera = camera; }

    static void OnFrameBegin();
    static void OnFrameEnd();

private:
    static Window* m_MainWindow;
    static Camera* m_MainCamera;
};