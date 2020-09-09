#pragma once
#include "geometry/vector3.h"
#include <string>

class Transform : public Component
{
public:
	Transform(Entity* owner) : Component(owner)
	{
		name = "Transform";
	}

	Vector3 localPosition;
};