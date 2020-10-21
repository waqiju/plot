#pragma once
#include <string>
#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"
#include "glm/glm.hpp"


class Matrix4x4
{
public:
    static constexpr  size_t Length() { return 4; }
    static Matrix4x4 identity;
	static Matrix4x4 TRS(Vector3 position, Quaternion rotation, Vector3 scale);
    static Matrix4x4 Translate(Vector3 position);
    static Matrix4x4 Rotate(Quaternion q);
    static Matrix4x4 LookAt(Vector3 eye, Vector3 center, Vector3 up);
    static Matrix4x4 LookAtRH(Vector3 eye, Vector3 center, Vector3 up);
    static Matrix4x4 Perspective(float fov, float aspect, float zNear, float zFar);
    static Matrix4x4 PerspectiveRH(float fov, float aspect, float zNear, float zFar);
    static Matrix4x4 AngleAxis(float angle, Vector3 axis);

	Matrix4x4();
    Matrix4x4(float x);
    Matrix4x4(float a1, float a2, float a3, float a4,
        float b1, float b2, float b3, float b4,
        float c1, float c2, float c3, float c4,
        float d1, float d2, float d3, float d4);
    Matrix4x4(Vector4 column1, Vector4 column2, Vector4 column3, Vector4 column4);
    Matrix4x4(Vector3 position, Quaternion rotation, Vector3 scale);
    explicit Matrix4x4(glm::mat4x4 m) { (*this) = *(reinterpret_cast<Matrix4x4*>(&m)); };

    Vector4& operator[](size_t i);
    Vector4 const& operator[](size_t i) const;
    Matrix4x4& operator*=(Matrix4x4 const& m);

    Matrix4x4 Transpose() const;
    Matrix4x4 Inverse() const;
    float Determinant() const;
    void Decompose(Vector3& position, Quaternion& rotation, Vector3& scaling) const;
    Vector3 DecomposePosition() const;

    Vector3 MultiplyPoint(Vector3 v) const;
    Vector3 MultiplyPoint3x4(Vector3 v) const;

    std::string ToString() const;

private:
    Vector4 m_Value[4];
};

Matrix4x4 operator*(Matrix4x4 const& lhs, Matrix4x4 const& rhs);
Vector4 operator*(Matrix4x4 const& m, Vector4 const& v);
