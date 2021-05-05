#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "gui/hover_tips_window.h"
#include <application/application.h>


namespace GUI
{


static bool show_demo_window = false;
static bool show_app_metrics = false;
static bool show_hover_tips_window = true;

static bool show_ma5 = false;
static bool show_ma8 = false;
// ä¸­
static bool show_ma13 = false;
static bool show_ma21 = false;
static bool show_ma34 = false;
static bool show_ma55 = false;
static bool show_ma89 = false;



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

}