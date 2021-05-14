#pragma once
#include <string>


namespace GUI
{

class MenuItemFloatingPanel
{
public:
    MenuItemFloatingPanel(int panelIndex)
    {
        m_PanelIndex = panelIndex;
        m_PanelMenuName = "Panel-" +  std::to_string(panelIndex);
    }

    void Show();

private:
    int m_PanelIndex=0;
    std::string m_PanelMenuName;

};

}
