#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "gui/hover_tips_window.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_layout.h"
#include <string>


namespace GUI
{


static std::string style = "style_panel_2";


void ResetLayoutStyle();


void ShowMenuItemLayout()
{
    if (ImGui::BeginMenu("Layout"))
    {
        if (ImGui::MenuItem("Style Panel 2", NULL, style=="style_panel_2"))
        {
            style = "style_panel_2";
            ResetLayoutStyle();
        }
        if (ImGui::MenuItem("Style Panel 4", NULL, style=="style_panel_4"))
        {
            style = "style_panel_4";
            ResetLayoutStyle();
        }
        ImGui::EndMenu();
    }
}


void ResetLayoutStyle()
{
    CommandRequest request;
    request.set_name("reset_layout_style");
    auto parameter = request.add_parameters();
    parameter->set_p_string(style) ;
    ChimeraClient::client->SubmitCommand(request);
}


}