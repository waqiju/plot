#include "world_object.h"

int WorldObject::GetInstanceID()
{
    return 0;
}

int WorldObject::GetHashCode()
{
    return (int)this;
}

std::string WorldObject::ToString()
{
    return std::string();
}
