#pragma once


class Vector4
{
public:
	static Vector4 zero;
	static Vector4 one;

	float x, y, z, w;
	Vector4() :Vector4(0, 0, 0, 0) {}
	Vector4(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}

	float Magnitude();
    void Normalize();

	constexpr static size_t Length() { return 4; }
	float& operator[](size_t i);
    float const& operator[](size_t i) const;
};

Vector4 operator+(Vector4 lhs, Vector4 rhs);
Vector4 operator-(Vector4 lhs, Vector4 rhs);
Vector4 operator*(Vector4 v, float scalar);