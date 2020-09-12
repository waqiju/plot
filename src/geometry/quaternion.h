#pragma once


class Quaternion
{
public:
	static Quaternion identity;

	float x, y, z, w;
	Quaternion() :Quaternion(0, 0, 0, 0) {}
	Quaternion(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}
};
