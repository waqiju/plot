#include "world_object.h"

int WorldObject::GetInstanceID()
{
    return id;
}

int WorldObject::GetHashCode()
{
    return (int)this;
}

std::string WorldObject::ToString()
{
    return std::string();
}
