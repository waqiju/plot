#pragma once


class Vector3
{
public:
	static Vector3 zero;
	static Vector3 one;

	float x, y, z;
	Vector3() :Vector3(0, 0, 0) {}
	Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}
};

Vector3 Vector3::zero = Vector3(0, 0, 0);
Vector3 Vector3::one = Vector3(1, 1, 1);