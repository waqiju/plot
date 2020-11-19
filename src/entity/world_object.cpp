#include "world_object.h"


WorldObject::WorldObject()
{
    id = ObjectID::NextAutoID(this);
}

// WorldObject::~WorldObject()
// {
//     id.Unregister();
// }

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
