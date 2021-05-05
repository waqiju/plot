#pragma once

struct GLFWwindow;

class ImGuiFacade
{
public:
    static void Initialize(GLFWwindow* window);
    static void Shutdown();
    static void OnFrameBegin();
    static void OnFrameSwapBuffersBefore();

private:
    static bool s_ShowDemoWindow;
};