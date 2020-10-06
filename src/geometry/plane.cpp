#include "plane.h"
#include <sstream>


std::string Plane::ToString() const
{
	std::stringstream stream;
	stream << "Plane [" << point.ToString() << ", " << normal.ToString() << "]";
	return stream.str();
}
