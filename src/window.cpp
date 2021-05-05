#include "window.h"
#include "application/application.h"
#include <iostream>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


bool Window::s_GlfwInitialized = false;
std::map<GLFWwindow*, Window*> Window::s_GLFWwindowToWindow;


// TODO: 当前实现只支持创建一个窗口
// 支持多个窗口需要处理 glfw 和 glad 的依赖，后面有空在看了
Window* Window::Create(std::string title, unsigned int width, unsigned int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (glfwWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1); // Enable vsync
    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to intialize GLAD" << std::endl;
        return NULL;
    }

    Window* window = new Window(glfwWindow);
    s_GLFWwindowToWindow[glfwWindow] = window;
    // buffer size callback
    glfwSetFramebufferSizeCallback(glfwWindow, Window::FramebufferSizeCallback);
    // mouse scroll changed
    glfwSetScrollCallback(glfwWindow, Window::MouseScrollCallback);
    // application
    Application::SetMainWindow(window);

    return window;
}

void Window::FramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
    Window* window = s_GLFWwindowToWindow[glfwWindow];
    if (window == NULL || !window->onWindowSizeChanged)
        return;

    window->onWindowSizeChanged(window, width, height);
}

void Window::MouseScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset)
{
    if (Input::IsMouseInGui())
    {
        return;
    }

    Window* window = s_GLFWwindowToWindow[glfwWindow];
    if (window == NULL || !window->onMouseScrollChanged)
        return;

    window->onMouseScrollChanged(window, xoffset, yoffset);
}

void Window::InitializeGlfw()
{
}

Window::Window(GLFWwindow* glfwWindow)
{
    m_GLFWwindow = glfwWindow;
}

void Window::FrameLoop(std::function<void()> onUpdated)
{
    // main loop
    while (!glfwWindowShouldClose(m_GLFWwindow))
    {
        glfwPollEvents();
        Application::OnFrameBegin();
        onUpdated();

        Application::OnFrameSwapBuffersBefore();
        glfwSwapBuffers(m_GLFWwindow);
        Application::OnFrameEnd();
    }
}

void Window::Close()
{
    glfwTerminate();
}

bool Window::IsWindowIconic()
{
    HWND handle = glfwGetWin32Window(m_GLFWwindow);
    return IsIconic(handle);
}
