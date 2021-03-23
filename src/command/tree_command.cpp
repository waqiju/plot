#include "tree_command.h"
#include "entity/ec.h"
#include "plot/plot.h"
#include <iostream>

void TravelEntity(Entity* entity, int indentLevel, std::string& content);


TreeCommand::TreeCommand(int id)
{
    m_Id = id;
}


std::string TreeCommand::Execute()
{
	std::string content;
	if (m_Id == 0)
	{
		content += "<*>\n";
		for (auto& entity : World::ActiveWorld()->GetRootEntities())
		{
			TravelEntity(entity, 1, content);
		}
	}
	else
	{
		auto startObj = ObjectID::Find(m_Id);
		if (startObj == nullptr)
		{
			return "<empty>";
		}
		auto startEntity = startObj->ToEntity();
		if (startEntity == nullptr)
		{
			return "<empty>";
		}
		TravelEntity(startEntity, 0, content);
	}
	

    return content;
}

void TravelEntity(Entity* entity, int indentLevel, std::string& content)
{
	// Self
	std::string leading;
	if (indentLevel > 0)
	{
		leading = std::string((indentLevel - 1) * 4, ' ') + "|-- ";
	}
	content += leading + entity->name + "\n";
	// Children
	for (auto& childTr : entity->GetTransform()->Children())
	{
		TravelEntity(childTr->OwnerEntity(), indentLevel + 1, content);
	}
}