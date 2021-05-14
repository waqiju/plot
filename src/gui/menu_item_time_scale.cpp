#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_time_scale.h"


namespace GUI
{


static int time_scale = -1;


void ResetTimeScale(int time_scale);


void ShowMenuItemTimeScale()
{
    if (ImGui::BeginMenu("Time-Scale"))
    {
        if (ImGui::MenuItem("Daily", NULL, time_scale==0))
        {
            time_scale = 0;
            ResetTimeScale(time_scale);
        }
        if (ImGui::MenuItem("Weekly", NULL, time_scale==1))
        {
            time_scale = 1;
            ResetTimeScale(time_scale);
        }
        if (ImGui::MenuItem("Monthly", NULL, time_scale==2))
        {
            time_scale = 2;
            ResetTimeScale(time_scale);
        }
        ImGui::EndMenu();
    }
}


void ResetTimeScale(int time_scale)
{
    CommandRequest request;
    request.set_name("reset_time_scale");
    auto parameter = request.add_parameters();
    parameter->set_p_int(time_scale);
    ChimeraClient::client->SubmitCommand(request);
}


}