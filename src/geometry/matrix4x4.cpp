#include <cassert>
#include <cmath>
#include <limits>
#include "matrix4x4.h"


Matrix4x4 Matrix4x4::TRS(Vector3 position, Quaternion rotation, Vector3 scale)
{
    Matrix4x4 m = Rotate(rotation);

    m[0][0] = m[0][0] * scale.x;
    m[1][0] = m[1][0] * scale.x;
    m[2][0] = m[2][0] * scale.x;
    m[3][0] = position.x;

    m[0][1] = m[0][1] * scale.x;
    m[1][1] = m[1][1] * scale.x;
    m[2][1] = m[2][1] * scale.x;
    m[3][1] = position.y;

    m[0][2] = m[0][2] * scale.x;
    m[1][2] = m[1][2] * scale.x;
    m[2][2] = m[2][2] * scale.x;
    m[3][2] = position.z;

    // m[3] is (0, 0, 0, 1), it set by Rotate. So skip it
    return m;
}

Matrix4x4 Matrix4x4::Translate(Vector3 position)
{
    Matrix4x4 result;
    result[3][0] = position.x;
    result[3][1] = position.y;
    result[3][2] = position.z;

    return result;
}

Matrix4x4 Matrix4x4::Rotate(Quaternion q)
{
    Matrix4x4 result;
    float& x = q.x;
    float& y = q.y;
    float& z = q.z;
    float& w = q.w;
    result[0][0] = 1.0f - 2.0f * (y * y + z * z);
    result[1][0] = 2.0f * (x * y - z * w);
    result[2][0] = 2.0f * (x * z + y * w);
    result[0][1] = 2.0f * (x * y + z * w);
    result[1][1] = 1.0f - 2.0f * (x * x + z * z);
    result[2][1] = 2.0f * (y * z - x * w);
    result[0][2] = 2.0f * (x * z - y * w);
    result[1][2] = 2.0f * (y * z + x * w);
    result[2][2] = 1.0f - 2.0f * (x * x + y * y);

    return result;
}

Matrix4x4 Matrix4x4::LookAt(Vector3 eye, Vector3 center, Vector3 up)
{
    Vector3 const f(Vector3::Normalize(center - eye));
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
    result[3][0] =-Vector3::Dot(s, eye);
    result[3][1] =-Vector3::Dot(u, eye);
    result[3][2] =-Vector3::Dot(f, eye);
    return result;
}

Matrix4x4 Matrix4x4::LookAtRH(Vector3 eye, Vector3 center, Vector3 up)
{
    Vector3 const f(Vector3::Normalize(center - eye));
    Vector3 const s(Vector3::Normalize(Vector3::Cross(f, up)));
    Vector3 const u(Vector3::Cross(s, f));

    Matrix4x4 result;
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;
    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;
    result[0][2] =-f.x;
    result[1][2] =-f.y;
    result[2][2] =-f.z;
    result[3][0] =-Vector3::Dot(s, eye);
    result[3][1] =-Vector3::Dot(u, eye);
    result[3][2] =Vector3::Dot(f, eye);
    return result;
}

// 对应 glm 的 perspectiveLH_NO
Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float zNear, float zFar)
{
    assert(abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0));

    float const tanHalfFovy = tan(fov / static_cast<float>(2));

    Matrix4x4 Result(static_cast<float>(0));
    Result[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
    Result[1][1] = static_cast<float>(1) / (tanHalfFovy);
    Result[2][2] = (zFar + zNear) / (zFar - zNear);
    Result[2][3] = static_cast<float>(1);
    Result[3][2] = - (static_cast<float>(2) * zFar * zNear) / (zFar - zNear);
    return Result;
}

// 对应 glm 的 perspectiveRH_NO
Matrix4x4 Matrix4x4::PerspectiveRH(float fov, float aspect, float zNear, float zFar)
{
    assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0);

    float tanHalfFov = tan(fov / 2);

    Matrix4x4 result(0);
    result[0][0] = static_cast<float>(1) / (aspect * tanHalfFov);
    result[1][1] = static_cast<float>(1) / (tanHalfFov);
    result[2][2] = - (zFar + zNear) / (zFar - zNear);
    result[2][3] = - static_cast<float>(1);
    result[3][2] = - (static_cast<float>(2) * zFar * zNear) / (zFar - zNear);
    
    return result;
}

Matrix4x4 Matrix4x4::AngleAxis(float angle, Vector3 axis)
{
    float const a = angle;
    float const c = cos(a);
    float const s = sin(a);

    axis = Vector3::Normalize(axis);
    Vector3 temp(axis * (1 - c));

    Matrix4x4 result;
    result[0][0] = c + temp[0] * axis[0];
    result[0][1] = temp[0] * axis[1] + s * axis[2];
    result[0][2] = temp[0] * axis[2] - s * axis[1];

    result[1][0] = temp[1] * axis[0] - s * axis[2];
    result[1][1] = c + temp[1] * axis[1];
    result[1][2] = temp[1] * axis[2] + s * axis[0];

    result[2][0] = temp[2] * axis[0] + s * axis[1];
    result[2][1] = temp[2] * axis[1] - s * axis[0];
    result[2][2] = c + temp[2] * axis[2];

    return result;
}

Matrix4x4::Matrix4x4()
{
    (*this) = Matrix4x4::identity;
}

Matrix4x4::Matrix4x4(float x) :
    m_Value{Vector4(x, 0, 0, 0), Vector4(0, x, 0, 0), Vector4(0, 0, x, 0), Vector4(0, 0, 0, x)}
{}

Matrix4x4::Matrix4x4(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4, float d1, float d2, float d3, float d4):
    m_Value{Vector4(a1, a2, a3, a4), Vector4(b1, b2, b3, b4), Vector4(c1, c2, c3, c4), Vector4(d1, d2, d3, d4)}
{}

Matrix4x4::Matrix4x4(Vector3 translate, Quaternion rotation, Vector3 scale)
{
	(*this) = TRS(translate, rotation, scale);
}

Vector4& Matrix4x4::operator[](size_t i)
{
    assert(i >= 0 && i < this->Length());
    return m_Value[i];
}

Vector4 const& Matrix4x4::operator[](size_t i) const
{
    assert(i >= 0 && i < Length());
    return m_Value[i];
}

Matrix4x4& Matrix4x4::operator*=(Matrix4x4 const& m)
{
    return (*this = *this * m);
}

Matrix4x4 Matrix4x4::identity = Matrix4x4(1, 0, 0, 0, /* 0 */ 0, 1, 0, 0, /* 1 */ 0, 0, 1, 0, /* 2 */ 0, 0, 0, 1);


Matrix4x4 operator*(Matrix4x4 const& lhs, Matrix4x4 const& rhs)
{
    Vector4 const& SrcA0 = lhs[0];
    Vector4 const& SrcA1 = lhs[1];
    Vector4 const& SrcA2 = lhs[2];
    Vector4 const& SrcA3 = lhs[3];

    Vector4 const& SrcB0 = rhs[0];
    Vector4 const& SrcB1 = rhs[1];
    Vector4 const& SrcB2 = rhs[2];
    Vector4 const& SrcB3 = rhs[3];

    Matrix4x4 result;
    result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
    result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
    result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
    result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
    return result;
}
