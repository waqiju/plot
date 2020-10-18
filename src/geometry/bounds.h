#pragma once
#include "geometry/vector3.h"


class Bounds
{
public:
    Bounds(Vector3 inMin, Vector3 inMax);

    Vector3 min, max;

    Vector3 Center() const;
    Vector3 Size() const;
    Vector3 Extent() const;
    void Expand(Vector3 scaling);

    std::string ToString() const;
};