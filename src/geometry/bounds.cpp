#include "geometry/bounds.h"


Bounds::Bounds(Vector3 inMin, Vector3 inMax)
{
    min = inMin;
    max = inMax;    
}

Vector3 Bounds::Center() const
{
    return (min + max) / 2;
}

Vector3 Bounds::Size() const
{
    return max - min;
}

Vector3 Bounds::Extent() const
{
    return (max - min) / 2;
}
