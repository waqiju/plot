#pragma once
#include "vector3.h"
#include <string>


class Plane
{
public:
    static Plane XyPlane;

    Plane(Vector3 inPoint, Vector3 inNormal) : point(inPoint), normal(inNormal) {};
    std::string ToString() const;

    Vector3 point;
    Vector3 normal;
};