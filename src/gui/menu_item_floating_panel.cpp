#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_floating_panel.h"
#include "menu_item_cycle.h"


namespace GUI
{


void MenuItemFloatingPanel::Show()
{
    if (ImGui::BeginMenu(m_PanelMenuName.c_str()))
    {
        if (ImGui::MenuItem("Cycle Trend"))
        {
            CommandRequest request;
            request.set_name("draw_rating_cycle");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(GetCycleLevel());
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("Volume"))
        {
            CommandRequest request;
            request.set_name("draw_volume");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(1);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("Volume 5"))
        {
            CommandRequest request;
            request.set_name("draw_volume");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(5);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("Volume 20"))
        {
            CommandRequest request;
            request.set_name("draw_volume");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(20);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("Range"))
        {
            CommandRequest request;
            request.set_name("draw_true_range");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(1);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("Range 5"))
        {
            CommandRequest request;
            request.set_name("draw_true_range");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(5);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("Range 20"))
        {
            CommandRequest request;
            request.set_name("draw_true_range");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(20);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("RSI"))
        {
            CommandRequest request;
            request.set_name("draw_rsi");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(6);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("RSI HCLC"))
        {
            CommandRequest request;
            request.set_name("draw_rsi_hclc");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            parameter = request.add_parameters();
            parameter->set_p_int(6);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("High Velocity"))
        {
            CommandRequest request;
            request.set_name("draw_high_velocity");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            ChimeraClient::client->SubmitCommand(request);
        }
        if (ImGui::MenuItem("Low Velocity"))
        {
            CommandRequest request;
            request.set_name("draw_low_velocity");
            auto parameter = request.add_parameters();
            parameter->set_p_int(m_PanelIndex);
            ChimeraClient::client->SubmitCommand(request);
        }
        ImGui::EndMenu();
    }
}


}