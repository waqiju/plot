#include "top_menu_window.h"
#include "imgui_headers.h"
#include "hover_tips_window.h"
#include "menu_item_ma.h"
#include "menu_item_endpoint.h"
#include "menu_item_cycle.h"
#include "menu_item_floating_panel.h"
#include "menu_item_time_scale.h"
#include "menu_item_layout.h"
#include "application/application.h"
#include "chimera_client.h"


namespace GUI
{


static bool show_demo_window = false;
static bool show_app_metrics = false;
static bool show_hover_tips_window = true;
static MenuItemFloatingPanel panel_1 = MenuItemFloatingPanel(1);
static MenuItemFloatingPanel panel_2 = MenuItemFloatingPanel(2);
static MenuItemFloatingPanel panel_3 = MenuItemFloatingPanel(3);
static MenuItemFloatingPanel panel_4 = MenuItemFloatingPanel(4);


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
        ShowMenuItemLayout();
        ShowMenuItemTimeScale();
        panel_1.Show();
        panel_2.Show();
        panel_3.Show();
        panel_4.Show();
        ShowMenuItemMa();
        ShowMenuItemEndpoint();
        ShowMenuItemCycle();
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