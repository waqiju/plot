#include "destroy_entity_command.h"
#include "entity/ec.h"
#include <iostream>
#include "focus_plot_command.h"
#include "plot/plot.h"
#include "ui_helper.h"


FocusPlotCommand::FocusPlotCommand(float begin, float end)
{
    m_Begin = begin;
    m_End = end;
}


void FocusPlotCommand::Execute()
{
	MouseScrollHandlers::FocusPlot(m_Begin, m_End);
}