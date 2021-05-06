#include <application/imgui_facade.h>
#include "gl_headers.h"
#include "gui/top_menu_window.h"
#include "gui/imgui_headers.h"
#include "application/application.h"
#include "window.h"


void ImGuiFacade::Initialize(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiFacade::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiFacade::OnFrameBegin()
{
    // 窗口最小化时，跳过渲染 GUI
    if (Application::MainWindow()->IsWindowIconic())
    {
        return;
    }
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    GUI::ShowTopMenu();
}

void ImGuiFacade::OnFrameSwapBuffersBefore()
{
    if (Application::MainWindow()->IsWindowIconic())
    {
        return;
    }
    // ImGUI Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
