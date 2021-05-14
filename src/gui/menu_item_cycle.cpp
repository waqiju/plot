#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "gui/hover_tips_window.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_endpoint.h"


namespace GUI
{


static int cycle_level = 0;


void ShowMenuItemCycle()
{
    if (ImGui::BeginMenu("Cycle"))
    {
        if (ImGui::MenuItem("Level 0", NULL, cycle_level==0))
        {
            cycle_level = 0;
        }
        if (ImGui::MenuItem("Level 1", NULL, cycle_level==1))
        {
            cycle_level = 1;
        }
        if (ImGui::MenuItem("Level 2", NULL, cycle_level==2))
        {
            cycle_level = 2;
        }
        if (ImGui::MenuItem("Level 3", NULL, cycle_level==3))
        {
            cycle_level = 3;
        }
        ImGui::EndMenu();
    }
}

int GetCycleLevel()
{
    return cycle_level;
}



}