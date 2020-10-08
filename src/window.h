#pragma once
#include "gl_headers.h"
#include <string>
#include <map>
#include <functional>


class Window
{
public:
    static Window* CreateWindow(std::string title, unsigned int width = 800, unsigned int height = 600);
    static void FramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height);
    static void MouseScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
    static void InitializeGlfw();

    Window(GLFWwindow* glfwWindow);
    void FrameLoop(std::function<void()> onUpdated);
    void Close();
    GLFWwindow* RawGLFWwindow() { return m_GLFWwindow; }

    // window, width, height
    std::function<void(Window*, int, int)> onWindowSizeChanged = NULL;
    // Window* window, double xoffset, double yoffset
    std::function<void(Window*, double, double)> onMouseScrollChanged = NULL;

private:
    static std::map<GLFWwindow*, Window*> s_GLFWwindowToWindow;
    static bool s_GlfwInitialized;

    GLFWwindow* m_GLFWwindow;
    unsigned int m_Width;
    unsigned int m_Height;
};