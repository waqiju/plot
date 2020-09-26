#include "vector4.h"
#include <cmath>
#include <cassert>


Vector4 Vector4::zero = Vector4(0, 0, 0, 0);
Vector4 Vector4::one = Vector4(1, 1, 1, 1);


float Vector4::Length()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

float Vector4::Magnitude()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

void Vector4::Normalize()
{
	float l = Magnitude();
	if (l != 0)
	{
		x /= l;
		y /= l;
		z /= l;
        w /= l;
	}
}

float& Vector4::operator[](size_t i)
{
	assert(i >= 0 && i < ComponentSize());
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
}

float const& Vector4::operator[](size_t i) const
{
	assert(i >= 0 && i < ComponentSize());
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
}

Vector4 operator+(Vector4 lhs, Vector4 rhs)
{
    return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

Vector4 operator-(Vector4 lhs, Vector4 rhs)
{
    return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

Vector4 operator*(Vector4 v, float scalar)
{
    return Vector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}