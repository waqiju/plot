#include "time.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "application/application.h"


float Time::s_NowTime = 0;
float Time::s_DeltaTime = 0;
unsigned int Time::s_FrameCount = 0;

void Time::OnFrameBegin()
{
    ++s_FrameCount;

    float nowTime = glfwGetTime();
    s_DeltaTime = nowTime - s_NowTime;
    s_NowTime = nowTime;
}