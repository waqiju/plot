#pragma once
#include <string>
#include <glm/glm.hpp>


class Vector2
{
public:
	static size_t const ComponentSize() { return 2; }
	static Vector2 zero;
	static Vector2 one;
    static Vector2 Normalize(Vector2 value);
	static float Dot(Vector2 lhs, Vector2 rhs);

	float x, y;
	Vector2() :Vector2(0, 0) {}
	Vector2(float inX, float inY) : x(inX), y(inY) {}
	explicit Vector2(glm::vec3 vec) { (*this) = *(reinterpret_cast<Vector2*>(&vec)); }

	float Length();
    float Magnitude();
    void Normalize();

    float& operator[](size_t i);
	const float& operator[](size_t i) const;
    Vector2 operator-() const;
	Vector2& operator+=(const Vector2& v);

    std::string ToString() const;
};

Vector2 operator+(Vector2 lhs, Vector2 rhs);
Vector2 operator-(Vector2 lhs, Vector2 rhs);
Vector2 operator/(Vector2 v1, Vector2 v2);
Vector2 operator+(Vector2 v, float scalar);
Vector2 operator-(Vector2 v, float scalar);
Vector2 operator*(Vector2 v, float scalar);
Vector2 operator/(Vector2 v, float scalar);
bool operator==(Vector2 lhs, Vector2 rhs);
bool operator!=(Vector2 lhs, Vector2 rhs);
