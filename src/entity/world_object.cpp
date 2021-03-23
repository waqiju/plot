#include "world_object.h"
#include "entity.h"
#include "transform.h"


WorldObject::WorldObject()
{
    id = ObjectID::NextAutoID(this);
}

Entity* WorldObject::ToEntity()
{
	// 直接
	Entity* ptr = dynamic_cast<Entity*>(this);
	if (ptr != nullptr)
		return ptr;
	// Transform 转 Entity
	Transform* tr = dynamic_cast<Transform*>(this);
	if (tr != nullptr)
	{
		return tr->OwnerEntity();
	}

	return nullptr;
}

Transform* WorldObject::ToTransform()
{
	// 直接
	Transform* ptr = dynamic_cast<Transform*>(this);
	if (ptr != nullptr)
		return ptr;
	// Entity 转 Transform
	Entity* entity = dynamic_cast<Entity*>(this);
	if (entity != nullptr)
	{
		return entity->GetTransform();
	}

	return nullptr;
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
