#include <cmath>
#include "geometry/quaternion.h"


Quaternion Quaternion::identity = Quaternion(0, 0, 0, 1);


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