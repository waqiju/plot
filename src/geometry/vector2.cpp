#include "vector2.h"
#include <cmath>
#include <cassert>
#include <sstream>


Vector2 Vector2::zero = Vector2(0, 0);
Vector2 Vector2::one = Vector2(1, 1);


Vector2 Vector2::Normalize(Vector2 value)
{
    Vector2 result;
    float l = sqrt(value.x * value.x + value.y * value.y);
    if (l != 0)
    {
        result.x = value.x / l;
        result.y = value.y / l;
    }
    else
    {
        result = value;
    }
    return result;
}

float Vector2::Dot(Vector2 lhs, Vector2 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Vector2::Length()
{
	return sqrt(x * x + y * y);
}

float Vector2::Magnitude()
{
    return sqrt(x * x + y * y);
}

void Vector2::Normalize()
{
	float l = Magnitude();
	if (l != 0)
	{
		x /= l;
		y /= l;
	}
}


float& Vector2::operator[](size_t i)
{
    assert(i >= 0 && i < ComponentSize());
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    }
}

const float& Vector2::operator[](size_t i) const
{
    assert(i >= 0 && i < ComponentSize());
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    }
}

Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    this->x += v.x;
    this->y += v.y;
    return *this;
}

std::string Vector2::ToString() const
{
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}

Vector2 operator+(Vector2 lhs, Vector2 rhs)
{
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2 operator-(Vector2 lhs, Vector2 rhs)
{
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2 operator/(Vector2 v1, Vector2 v2)
{
    return Vector2(v1.x / v2.x, v1.y / v2.y);
}

Vector2 operator+(Vector2 v, float scalar)
{
    return Vector2(v.x + scalar, v.y + scalar);
}

Vector2 operator-(Vector2 v, float scalar)
{
    return Vector2(v.x - scalar, v.y - scalar);
}

Vector2 operator*(Vector2 v, float scalar)
{
    return Vector2(v.x * scalar, v.y * scalar);
}

Vector2 operator/(Vector2 v, float scalar)
{
    return Vector2(v.x / scalar, v.y / scalar);
}

bool operator==(Vector2 lhs, Vector2 rhs)
{
    return lhs.x == rhs.x
        && lhs.y == rhs.y;
}

bool operator!=(Vector2 lhs, Vector2 rhs)
{
    return !(lhs == rhs);
}
