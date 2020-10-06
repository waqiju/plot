#pragma once
#include "ray.h"
#include "plane.h"


class Physics
{
public:
    static bool Raycast(const Ray& ray, const Plane& plane, float& distance);
    static bool Raycast(const Ray& ray, const Plane& plane, Vector3& hitPoint);
};