#pragma once


class Vector3
{
public:
	static Vector3 zero;
	static Vector3 one;
    static Vector3 Normalize(Vector3 value);
	static float Dot(Vector3 lhs, Vector3 rhs);
    static Vector3 Cross(Vector3 lhs, Vector3 rhs);

	float x, y, z;
	Vector3() :Vector3(0, 0, 0) {}
	Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

	float Magnitude();
    void Normalize();

    size_t Length() { return 3; }
    float& operator[](size_t i);
};

Vector3 operator+(Vector3 lhs, Vector3 rhs);
Vector3 operator-(Vector3 lhs, Vector3 rhs);
Vector3 operator*(Vector3 v, float scalar);
