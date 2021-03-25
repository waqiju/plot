#include "destroy_entity_command.h"
#include "entity/ec.h"
#include <iostream>
#include "destroy_entity_command.h"
#include "reset_plot_command.h"
#include "plot/plot.h"


void ResetPlotCommand::Execute()
{
    auto object = World::ActiveWorld()->FindObject(ObjectID::PlotRoot);
    if (object == nullptr)
    {
        std::cout << "Object[" << ObjectID::PlotRoot << "] not found!\n";
        return;
    }

	//Transform* transform = object->ToTransform();
	//if (transform == nullptr)
	//{
	//	std::cout << "Should not go here!\n";
	//	return;
	//}

	//std::vector<Transform*> childrenCopy = transform->Children();
	//for (auto child : childrenCopy)
	//{
	//	auto entity = child->OwnerEntity();
	//	if (entity->tag != PlotHelper::kPlotEssentialTag)
	//	{
	//		delete entity;
	//	}
	//}

	Entity* entity = object->ToEntity();
	if (entity == nullptr)
	{
		std::cout << "Should not go here!\n";
		return;
	}
	delete entity;
	PlotHelper::CreatePlotRootEntity();
}