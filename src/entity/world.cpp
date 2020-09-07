#include "world.h"

World* World::ActiveWorld = NULL;

void World::CreateOneAsAcitve()
{
	if (ActiveWorld != NULL)
	{
		delete(ActiveWorld);
	}

	ActiveWorld = new World();
}
