#include "vector3.h"
#include <cmath>
#include <cassert>


Vector3 Vector3::zero = Vector3(0, 0, 0);
Vector3 Vector3::one = Vector3(1, 1, 1);


Vector3 Vector3::Normalize(Vector3 value)
{
    Vector3 result;
    float l = sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
    if (l != 0)
    {
        result.x = value.x / l;
        result.y = value.y / l;
        result.z = value.z / l;
    }
    else
    {
        result = value;
    }
    return result;
}

float Vector3::Dot(Vector3 lhs, Vector3 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3 Vector3::Cross(Vector3 lhs, Vector3 rhs)
{
    Vector3 result;
    result.x = lhs.y * rhs.z - lhs.z * rhs.y;
    result.y = lhs.z * rhs.x - lhs.x * rhs.z;
    result.z = lhs.x * rhs.y - lhs.y * rhs.x;

    return result;
}

float Vector3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

void Vector3::Normalize()
{
	float l = Magnitude();
	if (l != 0)
	{
		x /= l;
		y /= l;
		z /= l;
	}
}


float& Vector3::operator[](size_t i)
{
    assert(i >= 0 && i < Length());
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    }
}

const float& Vector3::operator[](size_t i) const
{
    assert(i >= 0 && i < Length());
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    }
}

Vector3 operator+(Vector3 lhs, Vector3 rhs)
{
    return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vector3 operator-(Vector3 lhs, Vector3 rhs)
{
    return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vector3 operator*(Vector3 v, float scalar)
{
    return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}

