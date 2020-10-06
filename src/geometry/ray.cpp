#include "ray.h"
#include <sstream>


std::string Ray::ToString() const
{
	std::stringstream stream;
	stream << "Ray [" << origin.ToString() << ", " << direction.ToString() << "]";
	return stream.str();
}

Vector3 Ray::GetPoint(float distance) const
{
    return origin + direction * distance;
}