#include "hover_tips_window.h"
#include "gui/imgui_headers.h"
#include "plot/hover_tips.h"
#include <vector>
#include <application/application.h>
#include <algorithm>


namespace GUI
{


std::vector<HoverTips*>  LocateHoverTipsComponentList();


void ShowHoverTipsWindow(bool* p_open)
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x-340, 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, -1), ImGuiCond_Always);

    ImGuiWindowFlags window_flags = 0;
    ImGui::Begin("Hover Tips", NULL, window_flags);
    auto tipsCpList = LocateHoverTipsComponentList();
    if (tipsCpList.size() <= 0)
    {
        ImGui::TextWrapped("--");
        ImGui::End();
        return;
    }

    std::sort(tipsCpList.begin(), tipsCpList.end(), [](auto a, auto b) {return a->priority < b->priority; });
    for (auto tipsCp : tipsCpList)
    {
        ImGui::TextWrapped(tipsCp->tips.c_str());
    }
    ImGui::End();
}

std::vector<HoverTips*> LocateHoverTipsComponentList()
{
    std::vector<HoverTips*> result;
    // mouse position in local
    Vector3 mousePosition = Input::MousePosition();
    Ray ray = Application::MainCamera()->ScreenPointToRay(mousePosition);
    Vector3 mouseWorldPosition;
    Physics::Raycast(ray, Plane::XyPlane, mouseWorldPosition);
    Transform* mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
    Vector3 mouseLocalPosition = mainPanelTr->WorldToLocalMatrix().MultiplyPoint3x4(mouseWorldPosition);

    // find tips
    auto tipsCpList = mainPanelTr->GetComponentsInChildren<HoverTips>();
    float bestDistance = 0.55f;
    for (auto tipsCp : tipsCpList)
    {
        float distance = Mathf::Abs(tipsCp->x - mouseLocalPosition.x);
        if (distance < bestDistance)
        {
            bestDistance = distance;
        }
    }
    for (auto tipsCp : tipsCpList)
    {
        float distance = Mathf::Abs(tipsCp->x - mouseLocalPosition.x);
        if (Mathf::Abs(distance - bestDistance) < 1e-4f)
        {
            result.push_back(tipsCp);
        }
    }

    return result;
}

}