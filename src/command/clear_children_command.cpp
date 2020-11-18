#include "clear_children_command.h"
#include "entity/ec.h"
#include <iostream>


ClearChildrenCommand::ClearChildrenCommand(int objectID)
{
    m_ObjectID = objectID;
}


void ClearChildrenCommand::Execute()
{
    auto object = World::ActiveWorld()->FindObject(m_ObjectID);
	Transform* transform = NULL;
	if (typeid(object) == (typeid(Transform*)))
	{
		transform = dynamic_cast<Transform*>(object);
	}
	else if (typeid(object) == typeid(Entity*))
	{
		transform = dynamic_cast<Entity*>(object)->GetComponent<Transform>();
	}
	else
	{
		std::cout<< "Should not go here!\n";
		return;
	}

	Entity* entity = transform->OnwerEntity();
	delete entity;
}