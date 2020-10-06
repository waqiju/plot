#pragma once
#include <string>
#include <glm/glm.hpp>


class Vector3
{
public:
	static size_t const ComponentSize() { return 3; }
	static Vector3 zero;
	static Vector3 one;
    static Vector3 Normalize(Vector3 value);
	static float Dot(Vector3 lhs, Vector3 rhs);
    static Vector3 Cross(Vector3 lhs, Vector3 rhs);

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

    std::string ToString();
};

Vector3 operator+(Vector3 lhs, Vector3 rhs);
Vector3 operator-(Vector3 lhs, Vector3 rhs);
Vector3 operator*(Vector3 v, float scalar);
Vector3 operator/(Vector3 v, float scalar);
bool operator==(Vector3 lhs, Vector3 rhs);
bool operator!=(Vector3 lhs, Vector3 rhs);
