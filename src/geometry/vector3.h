#pragma once


class Vector3
{
public:
	static Vector3 zero;
	static Vector3 one;

	float x, y, z;
	Vector3() :Vector3(0, 0, 0) {}
	Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

	float Magnitude();
    void Normalize();

    size_t Length() { return 3; }
    float& operator[](size_t i);
};
