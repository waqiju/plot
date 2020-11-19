#include "destroy_entity_command.h"
#include "entity/ec.h"
#include <iostream>
#include "destroy_entity_command.h"
#include "reset_plot_command.h"
#include "plot/plot.h"


void ResetPlotCommand::Execute()
{
	auto command = DestroyEntityCommand(ObjectID::PlotRoot);
	command.Execute();

    auto plotRootEntity = PlotHelper::CreatePlotRootEntity();
}