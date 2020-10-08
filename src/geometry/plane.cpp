#include "plane.h"
#include <sstream>


Plane Plane::XyPlane = Plane(Vector3::zero, Vector3(0, 0, 1));

std::string Plane::ToString() const
{
	std::stringstream stream;
	stream << "Plane [" << point.ToString() << ", " << normal.ToString() << "]";
	return stream.str();
}
