#include "input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "application/application.h"


bool Input::GetKey(int key)
{
    return glfwGetKey(Application::MainWindow()->RawGLFWwindow(), key) == GLFW_PRESS;
}

bool Input::GetKeyDown(int key)
{
    // TODO
    return false;
}

bool Input::GetKeyUp(int key)
{
    // TODO
    return false;
}
