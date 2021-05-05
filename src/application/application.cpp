#include <application/application.h>
#include <application/time.h>
#include "window.h"


std::string Application::ProjectPath = "../";
unsigned int Application::screenWidth = kDefaultScreenWidth;
unsigned int Application::screenHeight = kDefaultScreenHeight;
Window* Application::m_MainWindow = NULL;
Camera* Application::m_MainCamera = NULL;


void Application::OnApplicationBegin()
{
    ImGuiFacade::Initialize(m_MainWindow->RawGLFWwindow());
}

void Application::OnApplicationEnd()
{
    ImGuiFacade::Shutdown();
}

void Application::OnFrameBegin()
{
    Time::OnFrameBegin();
    ImGuiFacade::OnFrameBegin();
}

void Application::OnFrameSwapBuffersBefore()
{
    ImGuiFacade::OnFrameSwapBuffersBefore();
}

void Application::OnFrameEnd()
{
    Input::OnFrameEnd();
}