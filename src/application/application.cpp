#include <application/application.h>
#include <application/time.h>


std::string Application::ProjectPath = "../";
unsigned int Application::screenWidth = kDefaultScreenWidth;
unsigned int Application::screenHeight = kDefaultScreenHeight;
Window* Application::m_MainWindow = NULL;
Camera* Application::m_MainCamera = NULL;


void Application::OnFrameBegin()
{
    Time::OnFrameBegin();
}

void Application::OnFrameEnd()
{
    Input::OnFrameEnd();
}