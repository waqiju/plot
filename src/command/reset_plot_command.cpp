#include "destroy_entity_command.h"
#include "entity/ec.h"
#include <iostream>
#include "destroy_entity_command.h"
#include "reset_plot_command.h"
#include "plot/plot.h"


void ResetPlotCommand::Execute()
{
    auto object = World::ActiveWorld()->FindObject(ObjectID::PlotRoot);
    if (object == NULL)
    {
        std::cout << "Object[" << ObjectID::PlotRoot << "] not found!\n";
        return;
    }

	Transform* transform = NULL;
	if (typeid(*object) == (typeid(Transform)))
	{
		transform = dynamic_cast<Transform*>(object);
	}
	else if (typeid(*object) == typeid(Entity))
	{
		transform = dynamic_cast<Entity*>(object)->GetComponent<Transform>();
	}
	else
	{
		std::cout << "Should not go here!\n";
		return;
	}

	std::vector<Transform*> childrenCopy = transform->Children();
	for (auto child : childrenCopy)
	{
		auto entity = child->OwnerEntity();
		if (entity->tag != PlotHelper::kPlotEssentialTag)
		{
			delete entity;
		}
	}
}