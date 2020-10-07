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
    static const unsigned int kDefaultScreenWidth = 800;
    static const unsigned int kDefaultScreenHeight = 600;
    static unsigned int screenWidth;
    static unsigned int screenHeight;
    static Vector2 ScreenSize() { return Vector2(screenWidth, screenHeight); }

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