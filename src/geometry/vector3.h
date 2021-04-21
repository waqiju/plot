#pragma once
#include <string>
#include <glm/glm.hpp>


class Vector3
{
public:
	static size_t const ComponentSize() { return 3; }
	static const Vector3 zero;
	static const Vector3 one;
    static Vector3 Normalize(Vector3 value);
    static Vector3 Abs(Vector3 value);
	static float Dot(Vector3 lhs, Vector3 rhs);
    static Vector3 Cross(Vector3 lhs, Vector3 rhs);
	static Vector3 Lerp(Vector3 start, Vector3 end, float factor);

	float x, y, z;
	Vector3() :Vector3(0, 0, 0) {}
	Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}
	explicit Vector3(glm::vec3 vec) { (*this) = *(reinterpret_cast<Vector3*>(&vec)); }

	float Length();
    float Magnitude();
    void Normalize();

    float& operator[](size_t i);
	const float& operator[](size_t i) const;
    Vector3 operator-() const;
	Vector3& operator+=(const Vector3& v);

    std::string ToString() const;
};

Vector3 operator+(Vector3 lhs, Vector3 rhs);
Vector3 operator-(Vector3 lhs, Vector3 rhs);
Vector3 operator*(Vector3 v, float scalar);
Vector3 operator/(Vector3 v, float scalar);
Vector3 operator*(Vector3 v1, Vector3 v2);
Vector3 operator/(Vector3 v1, Vector3 v2);
bool operator==(Vector3 lhs, Vector3 rhs);
bool operator!=(Vector3 lhs, Vector3 rhs);
