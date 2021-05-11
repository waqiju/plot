#include "top_menu_window.h"
#include "gui/imgui_headers.h"
#include "gui/hover_tips_window.h"
#include "application/application.h"
#include "chimera_client.h"
#include "menu_item_ma.h"


namespace GUI
{


void DrawMa(int windowSize, bool show);
void DrawDefaultMa(int windowSize, bool show);
void ClearAllMa();


static bool show_default_ma5 = false;
static bool show_ma10 = false;
static bool show_ma20 = false;
static bool show_ma60 = false;
// 中
static bool show_ma5 = false;
static bool show_ma8 = false;
static bool show_ma13 = false;
static bool show_ma21 = false;
static bool show_ma34 = false;
static bool show_ma55 = false;
static bool show_ma89 = false;
// 高
static bool show_ma144 = false;
static bool show_ma233 = false;
static bool show_ma377 = false;
static bool show_ma610 = false;
static bool show_ma987 = false;



void ShowMenuItemMa()
{
    if (ImGui::BeginMenu("MA"))
    {
        if (ImGui::MenuItem("MA 5", NULL, &show_default_ma5))
        {
            DrawDefaultMa(5, show_default_ma5);
        }
        if (ImGui::MenuItem("MA 10", NULL, &show_ma10))
        {
            DrawDefaultMa(10, show_ma10);
        }
        if (ImGui::MenuItem("MA 20", NULL, &show_ma20))
        {
            DrawDefaultMa(20, show_ma20);
        }
        if (ImGui::MenuItem("MA 60", NULL, &show_ma60))
        {
            DrawDefaultMa(60, show_ma60);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("MA 5", NULL, &show_ma5))
        {
            DrawMa(5, show_ma5);
        }
        if (ImGui::MenuItem("MA 8", NULL, &show_ma8))
        {
            DrawMa(8, show_ma8);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("MA 13", NULL, &show_ma13))
        {
            DrawMa(13, show_ma13);
        }
        if (ImGui::MenuItem("MA 21", NULL, &show_ma21))
        {
            DrawMa(21, show_ma21);
        }
        if (ImGui::MenuItem("MA 34", NULL, &show_ma34))
        {
            DrawMa(34, show_ma34);
        }
        if (ImGui::MenuItem("MA 55", NULL, &show_ma55))
        {
            DrawMa(55, show_ma55);
        }
        if (ImGui::MenuItem("MA 89", NULL, &show_ma89))
        {
            DrawMa(89, show_ma89);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("MA 144", NULL, &show_ma144))
        {
            DrawMa(144, show_ma144);
        }
        if (ImGui::MenuItem("MA 233", NULL, &show_ma233))
        {
            DrawMa(233, show_ma233);
        }
        if (ImGui::MenuItem("MA 377", NULL, &show_ma377))
        {
            DrawMa(377, show_ma377);
        }
        if (ImGui::MenuItem("MA 610", NULL, &show_ma610))
        {
            DrawMa(610, show_ma610);
        }
        if (ImGui::MenuItem("MA 987", NULL, &show_ma987))
        {
            DrawMa(987, show_ma987);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Clear All"))
        {
            ClearAllMa();
        }
        ImGui::EndMenu();
    }
}


void DrawMa(int windowSize, bool show)
{
    if (show)
    {
        CommandRequest request;
        request.set_name("draw_ma");
        auto parameter = request.add_parameters();
        parameter->set_p_int(windowSize);
        ChimeraClient::client->SubmitCommand(request);
    }
    else
    {
        CommandRequest request;
        request.set_name("remove_ma");
        auto parameter = request.add_parameters();
        parameter->set_p_int(windowSize);
        ChimeraClient::client->SubmitCommand(request);
    }
}


void DrawDefaultMa(int windowSize, bool show)
{
    if (show)
    {
        CommandRequest request;
        request.set_name("draw_default_ma");
        auto parameter = request.add_parameters();
        parameter->set_p_int(windowSize);
        ChimeraClient::client->SubmitCommand(request);
    }
    else
    {
        CommandRequest request;
        request.set_name("remove_ma");
        auto parameter = request.add_parameters();
        parameter->set_p_int(windowSize);
        ChimeraClient::client->SubmitCommand(request);
    }
}


void ClearAllMa()
{
    show_default_ma5 = false;
    show_ma10 = false;
    show_ma20 = false;
    show_ma60 = false;
    DrawDefaultMa(5, show_default_ma5);
    DrawDefaultMa(10, show_ma10);
    DrawDefaultMa(20, show_ma20);
    DrawDefaultMa(60, show_ma60);
    show_ma5 = false;
    show_ma8 = false;
    // 中
    show_ma13 = false;
    show_ma21 = false;
    show_ma34 = false;
    show_ma55 = false;
    show_ma89 = false;
    // 高
    show_ma144 = false;
    show_ma233 = false;
    show_ma377 = false;
    show_ma610 = false;
    show_ma987 = false;
    DrawMa(5, show_ma5);
    DrawMa(8, show_ma8);
    DrawMa(13, show_ma13);
    DrawMa(21, show_ma21);
    DrawMa(34, show_ma34);
    DrawMa(55, show_ma55);
    DrawMa(89, show_ma89);
    DrawMa(144, show_ma144);
    DrawMa(233, show_ma233);
    DrawMa(377, show_ma377);
    DrawMa(610, show_ma610);
    DrawMa(987, show_ma987);
}



}