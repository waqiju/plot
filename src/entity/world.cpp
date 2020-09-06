#include "world.h"

void World::CreateOneAsAcitve()
{
	if (ActiveWorld != NULL)
	{
		delete(ActiveWorld);
	}

	ActiveWorld = new World();
}
