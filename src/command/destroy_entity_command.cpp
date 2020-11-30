#include "destroy_entity_command.h"
#include "entity/ec.h"
#include <iostream>


DestroyEntityCommand::DestroyEntityCommand(int objectID)
{
    m_ObjectID = objectID;
}


void DestroyEntityCommand::Execute()
{
    auto object = World::ActiveWorld()->FindObject(m_ObjectID);
	if (object == NULL)
	{
		std::cout << "Object[" << m_ObjectID << "] not found!\n";
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
		std::cout<< "Should not go here!\n";
		return;
	}

	Entity* entity = transform->OwnerEntity();
	delete entity;
}