#pragma once
#include "geometry/vector3.h"


class Quaternion
{
public:
	static Quaternion identity;
    static Quaternion AngleAxis(float angle, Vector3 axis);

	float x, y, z, w;
	Quaternion() :Quaternion(0, 0, 0, 0) {}
	Quaternion(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}
};
