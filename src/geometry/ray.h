#pragma once
#include "vector3.h"
#include <string>


class Ray
{
public:
    Ray(Vector3 inOrigin, Vector3 inDirection) : origin(inOrigin), direction(inDirection) {};
    Vector3 GetPoint(float distance) const;
    std::string ToString() const;

    Vector3 origin;
    Vector3 direction;
};