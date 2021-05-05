#pragma once
#include <vector>
#include "geometry/vector3.h";
#include "graphic/color.h";


const int kStipplePatternDefault = ~(0);


void ApplyLineStipple(const std::vector<Vector3>& source, const std::vector<Color>& sourceColor, std::vector<Vector3>& destination, std::vector<Color>& destinationColor, int pattern);