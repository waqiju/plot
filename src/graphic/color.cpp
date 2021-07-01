#include "color.h"
#include "geometry/mathf.h"
#include <sstream>


const Color Color::white = Color(1, 1, 1);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(1, 0, 0);
const Color Color::green = Color(0, 1, 0);
const Color Color::blue = Color(0, 0, 1);
const Color Color::grey = Color(0.5, 0.5, 0.5);
const Color Color::kBackgroundWhite = Color(1, 1, 1, 0.15f);


std::string Color::ToString() const
{
	std::stringstream stream;
	stream << "Color(" << r << ", " << g << ", " << b << ")";
	return stream.str();
}


Color Color::Lerp(Color start, Color end, float factor)
{
    return Color(
            Mathf::Lerp(start.r, end.r, factor),
            Mathf::Lerp(start.g, end.g, factor),
            Mathf::Lerp(start.b, end.b, factor),
            Mathf::Lerp(start.a, end.a, factor)
        );
}