#include "physics.h"
#include "mathf.h"


bool Physics::Raycast(const Ray& ray, const Plane& plane, float& distance)
{
    const float n_dot_u = Vector3::Dot(plane.normal, ray.direction);
    if (Mathf::IsZero(n_dot_u))
    {
        return false;
    }

    distance = Vector3::Dot(plane.normal, plane.point) - Vector3::Dot(plane.normal, ray.origin);
    distance /= n_dot_u;
    return distance >= 0;
}

bool Physics::Raycast(const Ray& ray, const Plane& plane, Vector3& hitPoint)
{
    float distance;
    if (!Raycast(ray, plane, distance))
    {
        return false;
    }

    hitPoint = ray.GetPoint(distance);
    return true;
}