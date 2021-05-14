﻿#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "gui/hover_tips_window.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_endpoint.h"


namespace GUI
{


void DrawRatingEndpoint(int level);


void ShowMenuItemEndpoint()
{
    if (ImGui::BeginMenu("Endpoint"))
    {
        if (ImGui::MenuItem("Level 0"))
        {
            DrawRatingEndpoint(0);
        }
        if (ImGui::MenuItem("Level 1"))
        {
            DrawRatingEndpoint(1);
        }
        if (ImGui::MenuItem("Level 2"))
        {
            DrawRatingEndpoint(2);
        }
        if (ImGui::MenuItem("Level 3"))
        {
            DrawRatingEndpoint(3);
        }
        if (ImGui::MenuItem("Clear"))
        {
            DrawRatingEndpoint(4);
        }
        ImGui::EndMenu();
    }
}


void DrawRatingEndpoint(int level)
{
    CommandRequest request;
    request.set_name("draw_rating_endpoint");
    auto parameter = request.add_parameters();
    parameter->set_p_int(level);
    ChimeraClient::client->SubmitCommand(request);
}


}