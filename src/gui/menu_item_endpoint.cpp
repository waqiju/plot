#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "gui/hover_tips_window.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_endpoint.h"


namespace GUI
{


void DrawMaEndpoint(int level);
void DrawSegmentEndpoint();
void DrawTigerEndpoint(int windowSize);
void DrawVolumeEndpoint(bool includeC);
void ClearSegmentEndpoint();
void ClearTigerEndpoint();
void ClearVolumeEndpoint();


void ShowMenuItemEndpoint()
{
    if (ImGui::BeginMenu("Endpoint"))
    {
        if (ImGui::MenuItem("Level 0"))
        {
            DrawMaEndpoint(0);
        }
        if (ImGui::MenuItem("Level 1"))
        {
            DrawMaEndpoint(1);
        }
        if (ImGui::MenuItem("Level 2"))
        {
            DrawMaEndpoint(2);
        }
        if (ImGui::MenuItem("Level 3"))
        {
            DrawMaEndpoint(3);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Segment"))
        {
            DrawSegmentEndpoint();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Tiger 5"))
        {
            DrawTigerEndpoint(5);
        }
        if (ImGui::MenuItem("Tiger 20"))
        {
            DrawTigerEndpoint(20);
        }
        if (ImGui::MenuItem("Tiger 60"))
        {
            DrawTigerEndpoint(60);
        }
        if (ImGui::MenuItem("Tiger 100"))
        {
            DrawTigerEndpoint(100);
        }
        if (ImGui::MenuItem("Tiger 200"))
        {
            DrawTigerEndpoint(200);
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
            DrawMaEndpoint(4);
            ClearSegmentEndpoint();
            ClearTigerEndpoint();
            ClearVolumeEndpoint();
        }
        ImGui::EndMenu();
    }
}


void DrawMaEndpoint(int level)
{
    CommandRequest request;
    request.set_name("draw_ma_endpoint");
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


void DrawTigerEndpoint(int windowSize)
{
    CommandRequest request;
    request.set_name("draw_tiger_endpoint");
    auto parameter = request.add_parameters();
    parameter->set_p_int(windowSize);
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


void ClearTigerEndpoint()
{
    CommandRequest request;
    request.set_name("clear_tiger_endpoint");
    ChimeraClient::client->SubmitCommand(request);
}


void ClearVolumeEndpoint()
{
    CommandRequest request;
    request.set_name("clear_volume_endpoint");
    ChimeraClient::client->SubmitCommand(request);
}


}