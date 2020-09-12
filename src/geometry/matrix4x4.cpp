#include <cassert>
#include "matrix4x4.h"


Matrix4x4 Matrix4x4::TRS(Vector3 position, Quaternion rotation, Vector3 scale)
{
    Matrix4x4 m = Rotate(rotation);

    m[0][0] = m[0][0] * scale.x;
    m[0][1] = m[0][1] * scale.x;
    m[0][2] = m[0][2] * scale.x;
    m[0][3] = position.x;

    m[1][0] = m[1][0] * scale.x;
    m[1][1] = m[1][1] * scale.x;
    m[1][2] = m[1][2] * scale.x;
    m[1][3] = position.y;

    m[2][0] = m[2][0] * scale.x;
    m[2][1] = m[2][1] * scale.x;
    m[2][2] = m[2][2] * scale.x;
    m[2][3] = position.y;

    // m[3] is (0, 0, 0, 1), it set by Rotate. So skip it
    return m;
}

Matrix4x4 Matrix4x4::Rotate(Quaternion q)
{
    Matrix4x4 matrix;
    float& x = q.x;
    float& y = q.y;
    float& z = q.z;
    float& w = q.w;
    matrix[0][0] = 1.0f - 2.0f * (y * y + z * z);
    matrix[0][1] = 2.0f * (x * y - z * w);
    matrix[0][2] = 2.0f * (x * z + y * w);
    matrix[1][0] = 2.0f * (x * y + z * w);
    matrix[1][1] = 1.0f - 2.0f * (x * x + z * z);
    matrix[1][2] = 2.0f * (y * z - x * w);
    matrix[2][0] = 2.0f * (x * z - y * w);
    matrix[2][1] = 2.0f * (y * z + x * w);
    matrix[2][2] = 1.0f - 2.0f * (x * x + y * y);

    return matrix;
}

Matrix4x4 Matrix4x4::LookAt(Vector3 from, Vector3 to, Vector3 up)
{
    Vector3 const f(Vector3::Normalize(to - from));
    Vector3 const s(Vector3::Normalize(Vector3::Cross(up, f)));
    Vector3 const u(Vector3::Cross(f, s));

    Matrix4x4 result;
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;
    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;
    result[0][2] = f.x;
    result[1][2] = f.y;
    result[2][2] = f.z;
    result[3][0] = -Vector3::Dot(s, from);
    result[3][1] = -Vector3::Dot(u, from);
    result[3][2] = -Vector3::Dot(f, from);
    return result;
}

Matrix4x4::Matrix4x4()
{
    (*this) = Matrix4x4::identity;
}

Matrix4x4::Matrix4x4(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4, float d1, float d2, float d3, float d4)
{
    m_Value[0] = Vector4(a1, a2, a3, a4);
    m_Value[1] = Vector4(b1, b2, b3, b4);
    m_Value[2] = Vector4(c1, c2, c3, c4);
    m_Value[3] = Vector4(d1, d2, d3, d4);
}

Matrix4x4::Matrix4x4(Vector3 translate, Quaternion rotation, Vector3 scale)
{
	(*this) = TRS(translate, rotation, scale);
}

Vector4& Matrix4x4::operator[](size_t i)
{
    assert(i >= 0 && i < this->Length());
    return m_Value[i];
}

Matrix4x4 Matrix4x4::identity = Matrix4x4(1, 0, 0, 0, /* 0 */ 0, 1, 0, 0, /* 1 */ 0, 0, 1, 0, /* 2 */ 0, 0, 0, 1);