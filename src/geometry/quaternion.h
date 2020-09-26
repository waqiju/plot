#pragma once
#include <string>


class Vector3;
class Matrix4x4;


class Quaternion
{
public:
	static Quaternion identity;
    static Quaternion AngleAxis(float angle, Vector3 axis);
    static Quaternion LookAt(Vector3 forward, Vector3 upwards);
    static Quaternion CastMatrixToQuaternion(Matrix4x4 m);

	float x, y, z, w;
	Quaternion() :Quaternion(0, 0, 0, 0) {}
	Quaternion(float inW, float inX, float inY, float inZ) : x(inX), y(inY), z(inZ), w(inW) {}

	Quaternion Inverse();
    Vector3 EulerAngles();
    float Roll();
    float Pitch();
    float Yaw();

    std::string ToString();
};
