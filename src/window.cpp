#include "window.h"
#include "application/application.h"
#include <iostream>

bool Window::s_GlfwInitialized = false;
std::map<GLFWwindow*, Window*> Window::s_GLFWwindowToWindow;


// TODO: 当前实现只支持创建一个窗口
// 支持多个窗口需要处理 glfw 和 glad 的依赖，后面有空在看了
Window* Window::CreateWindow(std::string title, unsigned int width, unsigned int height)
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
        Application::OnFrameBegin();
        onUpdated();

        glfwSwapBuffers(m_GLFWwindow);
        glfwPollEvents();
        Application::OnFrameEnd();
    }
}

void Window::Close()
{
    glfwTerminate();
}
