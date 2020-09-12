#pragma once
#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"


class Matrix4x4
{
public:
    static Matrix4x4 identity;
	static Matrix4x4 TRS(Vector3 translate, Quaternion rotation, Vector3 scale);
    static Matrix4x4 Rotate(Quaternion q);
    static Matrix4x4 LookAt(Vector3 from, Vector3 to, Vector3 up);
    static Matrix4x4 Perspective(float fov, float aspect, float zNear, float zFar);

	Matrix4x4();
    Matrix4x4(float x);
    Matrix4x4(float a1, float a2, float a3, float a4,
        float b1, float b2, float b3, float b4,
        float c1, float c2, float c3, float c4,
        float d1, float d2, float d3, float d4);
    Matrix4x4(Vector3 position, Quaternion rotation, Vector3 scale);

    size_t Length() {return 4;}
    Vector4& operator[](size_t i);

private:
    Vector4 m_Value[4];
	// Matrix4x4() :Vector3(0, 0, 0) {}
	// Matrix4x4(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}
};
