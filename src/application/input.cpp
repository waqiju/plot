#include "input.h"
#include "gl_headers.h"
#include "gui/imgui_headers.h"
#include "window.h"
#include "application/application.h"
#include "geometry/geometry.h"


std::map<int, bool> Input::s_PreviousKeyDown;
std::map<int, bool> Input::s_PreviousMouseButtonDown;


bool Input::GetKey(int key)
{
    return glfwGetKey(Application::MainWindow()->RawGLFWwindow(), key) == GLFW_PRESS;
}

bool Input::GetKeyDown(int key)
{
    bool nowIsDown = GetKey(key);
    if (nowIsDown)
    {
        bool previousIsDown = s_PreviousKeyDown.find(key) != s_PreviousKeyDown.end() && s_PreviousKeyDown[key];
        bool isChanged = nowIsDown != previousIsDown;
        return isChanged;
    }

    return false;
}

bool Input::GetKeyUp(int key)
{
    bool nowIsDown = GetKey(key);
    if (!nowIsDown)
    {
        bool previousIsDown = s_PreviousKeyDown.find(key) != s_PreviousKeyDown.end() && s_PreviousKeyDown[key];
        bool isChanged = nowIsDown != previousIsDown;
        return isChanged;
    }

    return false;
}

bool Input::GetMouseButton(int button)
{
    if (IsMouseInGui())
    {
        return false;
    }
    return glfwGetMouseButton(Application::MainWindow()->RawGLFWwindow(), button) == GLFW_PRESS;
}

bool Input::GetMouseButtonDown(int button)
{
    bool nowIsDown = GetMouseButton(button);
    if (nowIsDown)
    {
        bool previousIsDown = s_PreviousMouseButtonDown.find(button) != s_PreviousMouseButtonDown.end() && s_PreviousMouseButtonDown[button];
        bool isChanged = nowIsDown != previousIsDown;
        return isChanged;
    }

    return false;
}

bool Input::GetMouseButtonUp(int button)
{
    bool nowIsDown = GetMouseButton(button);
    if (!nowIsDown)
    {
        bool previousIsDown = s_PreviousMouseButtonDown.find(button) != s_PreviousMouseButtonDown.end() && s_PreviousMouseButtonDown[button];
        bool isChanged = nowIsDown != previousIsDown;
        return isChanged;
    }

    return false;
}

Vector3 Input::MousePosition()
{
    double x, y;
    glfwGetCursorPos(Application::MainWindow()->RawGLFWwindow(), &x, &y);
    return Vector3((float)(x), (float)(y), 0);
}

bool Input::IsMouseInGui()
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

void Input::OnFrameEnd()
{
    for (int i = 0;i <= GLFW_KEY_LAST;++i)
    {
        bool isDown = glfwGetKey(Application::MainWindow()->RawGLFWwindow(), i) == GLFW_PRESS;
        s_PreviousKeyDown[i] = isDown;
    }
    for (int i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_MIDDLE; ++i)
    {
        bool isDown = glfwGetMouseButton(Application::MainWindow()->RawGLFWwindow(), i) == GLFW_PRESS;
        s_PreviousMouseButtonDown[i] = isDown;
    }
}
