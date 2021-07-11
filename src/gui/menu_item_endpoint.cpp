﻿#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "gui/hover_tips_window.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_endpoint.h"


namespace GUI
{


void DrawRatingEndpoint(int level);
void DrawSegmentEndpoint();
void DrawVolumeEndpoint(bool includeC);
void ClearSegmentEndpoint();
void ClearVolumeEndpoint();


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
        ImGui::Separator();
        if (ImGui::MenuItem("Segment"))
        {
            DrawSegmentEndpoint();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Volume"))
        {
            DrawVolumeEndpoint(false);
        }
        if (ImGui::MenuItem("Volume Include C"))
        {
            DrawVolumeEndpoint(true);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Clear"))
        {
            DrawRatingEndpoint(4);
            ClearSegmentEndpoint();
            ClearVolumeEndpoint();
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


void DrawSegmentEndpoint()
{
    CommandRequest request;
    request.set_name("draw_segment_endpoint");
    ChimeraClient::client->SubmitCommand(request);
}


void DrawVolumeEndpoint(bool includeC)
{
    CommandRequest request;
    request.set_name("draw_volume_endpoint");
    auto parameter = request.add_parameters();
    parameter->set_p_int(4);
    parameter = request.add_parameters();
    parameter->set_p_bool(includeC);
    ChimeraClient::client->SubmitCommand(request);
}


void ClearSegmentEndpoint()
{
    CommandRequest request;
    request.set_name("clear_segment_endpoint");
    ChimeraClient::client->SubmitCommand(request);
}


void ClearVolumeEndpoint()
{
    CommandRequest request;
    request.set_name("clear_volume_endpoint");
    ChimeraClient::client->SubmitCommand(request);
}


}