#include <cassert>
#include <cmath>
#include <limits>
#include <sstream>
#include "matrix4x4.h"


Matrix4x4 Matrix4x4::TRS(Vector3 position, Quaternion rotation, Vector3 scale)
{
    Matrix4x4 m = Rotate(rotation);

    m[0][0] = m[0][0] * scale.x;
    m[1][0] = m[1][0] * scale.x;
    m[2][0] = m[2][0] * scale.x;
    m[3][0] = position.x;

    m[0][1] = m[0][1] * scale.y;
    m[1][1] = m[1][1] * scale.y;
    m[2][1] = m[2][1] * scale.y;
    m[3][1] = position.y;

    m[0][2] = m[0][2] * scale.z;
    m[1][2] = m[1][2] * scale.z;
    m[2][2] = m[2][2] * scale.z;
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

Matrix4x4::Matrix4x4(Vector4 column1, Vector4 column2, Vector4 column3, Vector4 column4):
    m_Value{column1, column2, column3, column4}
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

Matrix4x4 Matrix4x4::Transpose() const
{
    Matrix4x4 result;
    result[0][0] = m_Value[0][0];
    result[0][1] = m_Value[1][0];
    result[0][2] = m_Value[2][0];
    result[0][3] = m_Value[3][0];

    result[1][0] = m_Value[0][1];
    result[1][1] = m_Value[1][1];
    result[1][2] = m_Value[2][1];
    result[1][3] = m_Value[3][1];

    result[2][0] = m_Value[0][2];
    result[2][1] = m_Value[1][2];
    result[2][2] = m_Value[2][2];
    result[2][3] = m_Value[3][2];

    result[3][0] = m_Value[0][3];
    result[3][1] = m_Value[1][3];
    result[3][2] = m_Value[2][3];
    result[3][3] = m_Value[3][3];
    return result;
}

// func_matrix.inl  compute_inverse()
Matrix4x4 Matrix4x4::Inverse() const
{
    Vector4 const* m = &m_Value[0];
    float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

    float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
    float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

    float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
    float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

    float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
    float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

    float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
    float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

    float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

    Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
    Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
    Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
    Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
    Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
    Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

    Vector4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
    Vector4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
    Vector4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
    Vector4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

    Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
    Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
    Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
    Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

    Vector4 SignA(+1, -1, +1, -1);
    Vector4 SignB(-1, +1, -1, +1);
    Matrix4x4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

    Vector4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

    Vector4 Dot0(m[0] * Row0);
    float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

    float OneOverDeterminant = static_cast<float>(1) / Dot1;

    return Inverse * OneOverDeterminant;
}

float Matrix4x4::Determinant() const
{
    float SubFactor00 = m_Value[2][2] * m_Value[3][3] - m_Value[3][2] * m_Value[2][3];
    float SubFactor01 = m_Value[2][1] * m_Value[3][3] - m_Value[3][1] * m_Value[2][3];
    float SubFactor02 = m_Value[2][1] * m_Value[3][2] - m_Value[3][1] * m_Value[2][2];
    float SubFactor03 = m_Value[2][0] * m_Value[3][3] - m_Value[3][0] * m_Value[2][3];
    float SubFactor04 = m_Value[2][0] * m_Value[3][2] - m_Value[3][0] * m_Value[2][2];
    float SubFactor05 = m_Value[2][0] * m_Value[3][1] - m_Value[3][0] * m_Value[2][1];

    Vector4 DetCof(
        + (m_Value[1][1] * SubFactor00 - m_Value[1][2] * SubFactor01 + m_Value[1][3] * SubFactor02),
        - (m_Value[1][0] * SubFactor00 - m_Value[1][2] * SubFactor03 + m_Value[1][3] * SubFactor04),
        + (m_Value[1][0] * SubFactor01 - m_Value[1][1] * SubFactor03 + m_Value[1][3] * SubFactor05),
        - (m_Value[1][0] * SubFactor02 - m_Value[1][1] * SubFactor04 + m_Value[1][2] * SubFactor05));

    return
        m_Value[0][0] * DetCof[0] + m_Value[0][1] * DetCof[1] +
        m_Value[0][2] * DetCof[2] + m_Value[0][3] * DetCof[3];
}

void Matrix4x4::Decompose(Vector3& position, Quaternion& rotation, Vector3& scaling) const
{
    // translation
    position.x = m_Value[3][0];
    position.y = m_Value[3][1];
    position.z = m_Value[3][2];

    // columns
    Vector3 columns[3] = {
        Vector3(m_Value[0][0], m_Value[0][1], m_Value[0][2]),
        Vector3(m_Value[1][0], m_Value[1][1], m_Value[1][2]),
        Vector3(m_Value[2][0], m_Value[2][1], m_Value[2][2]),
    };

    // scaling
    scaling.x = columns[0].Length();
    scaling.y = columns[1].Length();
    scaling.z = columns[2].Length();

    if (Determinant() < 0)
    {
        scaling.x = -scaling.x;
        scaling.y = -scaling.y;
        scaling.z = -scaling.z;
    }

    // rotation
    if(scaling.x)
    {
        columns[0] = columns[0] / scaling.x;
    }
    if(scaling.y)
    {
        columns[1] = columns[1] / scaling.y;
    }
    if(scaling.z)
    {
        columns[2] = columns[2] / scaling.z;
    }

    Matrix4x4 rotationMatrix = Matrix4x4(
        Vector4(columns[0], 0),
        Vector4(columns[1], 0),
        Vector4(columns[2], 0),
        Vector4());
    rotation = Quaternion::CastMatrixToQuaternion(rotationMatrix);
}

Vector3 Matrix4x4::DecomposePosition() const
{
    return Vector3(m_Value[3][0], m_Value[3][1], m_Value[3][2]);
}

Vector3 Matrix4x4::MultiplyPoint(Vector3 v) const
{
    Vector4 result = (*this) * Vector4(v.x, v.y, v.z, 1);
    return Vector3(result.x / result.w, result.y / result.w, result.z / result.w);
}

Vector3 Matrix4x4::MultiplyPoint3x4(Vector3 v) const
{
    Vector3 result;
    const Vector4 (&m)[4] = m_Value;
    result[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0];
    result[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1];
    result[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2];
    return result;
}


std::string Matrix4x4::ToString() const
{
    std::stringstream stream;
    Vector3 t, s, e;
    Quaternion r;
    Decompose(t, r, s);
    e = r.EulerAngles();
    stream << "float[" << t.x << ", " << t.y << ", " << t.z << "], "
        << "R[" << e.x << ", " << e.y << ", " << e.z << "], "
        << "S[" << s.x << ", " << s.y << ", " << s.z << "]";
    return stream.str();
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

Vector4 operator*(Matrix4x4 const& m, Vector4 const& v)
{
    Vector4 result;
    result[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
    result[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
    result[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
    result[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
    return result;
}
