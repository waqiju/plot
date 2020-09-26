#include <cmath>
#include "geometry/quaternion.h"
#include "geometry/matrix4x4.h"
#include "geometry/mathf.h"
#include <sstream>


Quaternion Quaternion::identity = Quaternion(1, 0, 0, 0);


Quaternion Quaternion::AngleAxis(float angle, Vector3 axis)
{
    float const c = cos(angle * 0.5f);
    float const s = sin(angle * 0.5f);
    axis = Vector3::Normalize(axis);

    Quaternion result;
    result.x = axis.x * s;
    result.y = axis.y * s;
    result.z = axis.z * s;
    result.w = c;

    return result;
}

Quaternion Quaternion::LookAt(Vector3 forward, Vector3 upwards)
{
    // 应该用Matrix3x3的，偷懒，直接用Matrix4x4
    Matrix4x4 result(0);

    forward = Vector3::Normalize(forward);
    Vector3 right = Vector3::Normalize(Vector3::Cross(upwards, forward));
    Vector3 up = Vector3::Cross(forward, right);
    result[0] = Vector4(right, 0);
    result[1] = Vector4(up, 0);
    result[2] = Vector4(forward, 0);

    return CastMatrixToQuaternion(result);
}

Quaternion Quaternion::CastMatrixToQuaternion(Matrix4x4 m)
{
    float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
    float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
    float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
    float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

    int biggestIndex = 0;
    float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }
    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }
    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    float biggestVal = sqrt(fourBiggestSquaredMinus1 + static_cast<float>(1)) * static_cast<float>(0.5);
    float mult = static_cast<float>(0.25) / biggestVal;

    switch (biggestIndex)
    {
    case 0:
        return Quaternion(biggestVal, (m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult);
    case 1:
        return Quaternion((m[1][2] - m[2][1]) * mult, biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult);
    case 2:
        return Quaternion((m[2][0] - m[0][2]) * mult, (m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult);
    case 3:
        return Quaternion((m[0][1] - m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal);
    default: // Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
        assert(false);
        return Quaternion(1, 0, 0, 0);
    }
}

Quaternion Quaternion::Inverse()
{
    return Quaternion(w, -x, -y, -z);
}


Vector3 Quaternion::EulerAngles()
{
    return Vector3(Pitch(), Yaw(), Roll()) * Mathf::Rad2Deg;
}

float Quaternion::Roll()
{
    return static_cast<float>(::std::atan2(static_cast<float>(2) * (x * y + w * z), w * w + x * x - y * y - z * z));
}

float Quaternion::Pitch()
{
    float const y1 = static_cast<float>(2) * (y * z + w * x);
    float const x1 = w * w - x * x - y * y + z * z;

    if (Mathf::IsZero(x1) && Mathf::IsZero(y1)) //avoid atan2(0,0) - handle singularity - Matiis
        return static_cast<float>(static_cast<float>(2) * ::std::atan2(x, w));

    return static_cast<float>(::std::atan2(y1, x1));
}

float Quaternion::Yaw()
{
    return asin(Mathf::Clamp(static_cast<float>(-2) * (x * z - w * y), static_cast<float>(-1), static_cast<float>(1)));
}


std::string Quaternion::ToString()
{
    std::stringstream stream;
    Vector3 e = EulerAngles();
    stream << "EulerAngles[" << e.x << ", " << e.y << ", " << e.z << "]";
    return stream.str();
}