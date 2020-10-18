#include "bounds.h"
#include "geometry/bounds.h"
#include <sstream>


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

void Bounds::Expand(Vector3 scaling)
{
    Vector3 center = Center();
    min = center + (min - center) * scaling;
    max = center + (max - center) * scaling;
}

std::string Bounds::ToString() const
{
    std::stringstream ss;
    ss << "[Min" << min.ToString() << ", Max" << max.ToString() << "]";
    return ss.str();
}
