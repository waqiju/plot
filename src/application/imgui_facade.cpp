#include <application/imgui_facade.h>
#include "gui/hover_tips_window.h"
#include "gl_headers.h"
#include "gui/imgui_headers.h"


static bool show_demo_window = false;
static bool show_app_metrics = false;
static bool show_hover_tips_window = true;

static bool show_ma5 = false;
static bool show_ma8 = false;
// 中
static bool show_ma13 = false;
static bool show_ma21 = false;
static bool show_ma34 = false;
static bool show_ma55 = false;
static bool show_ma89 = false;


void ShowTopMenu();


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
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ShowTopMenu();
}

void ImGuiFacade::OnFrameSwapBuffersBefore()
{
    // ImGUI Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShowTopMenu()
{
    // FPS Window
    if (show_demo_window)       { ImGui::ShowDemoWindow(&show_demo_window); }
    if (show_app_metrics)       { ImGui::ShowMetricsWindow(&show_app_metrics); }
    if (show_hover_tips_window)       { GUI::ShowHoverTipsWindow(&show_hover_tips_window); }

    // Menu Window
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(20, 5), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x - 40, 10), ImGuiCond_Always);

    ImGui::Begin("Menu", NULL, window_flags);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("MA"))
        {
            ImGui::MenuItem("MA 5", NULL, &show_ma5);
            ImGui::MenuItem("MA 8", NULL, &show_ma8);
            ImGui::Separator();
            ImGui::MenuItem("MA 13", NULL, &show_ma13);
            ImGui::MenuItem("MA 21", NULL, &show_ma21);
            ImGui::MenuItem("MA 34", NULL, &show_ma34);
            ImGui::MenuItem("MA 55", NULL, &show_ma55);
            ImGui::MenuItem("MA 89", NULL, &show_ma89);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("ImGui Demo", NULL, &show_demo_window);
            ImGui::MenuItem("Metrics/Debugger", NULL, &show_app_metrics);
            ImGui::MenuItem("Hover Tips", NULL, &show_hover_tips_window);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}
