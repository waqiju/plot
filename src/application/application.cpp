#include <application/application.h>
#include <application/time.h>


std::string Application::ProjectPath = "../";
Window* Application::m_MainWindow = NULL;
Camera* Application::m_MainCamera = NULL;


void Application::OnFrameBegin()
{
    Time::OnFrameBegin();
}

void Application::OnFrameEnd()
{
    
}