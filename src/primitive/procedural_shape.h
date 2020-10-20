#pragma once
#include "geometry/geometry.h"
#include <vector>
#include "graphic/graphic.h"
#include "core_component/core_component.h"
#include "flexible_shape_mode.h"


class ProceduralShape
{
public:
    static int GenerateTriangle(const Vector3& v0, const Vector3& v1, std::vector<Vector3>& vertices);
    static void AppendColors(const Color& color, int count, std::vector<Color>& colors);
    static Bounds CalculateBounds(const Matrix4x4& matrix, const Vector3& center, FlexibleShapeMode mode, float& width, float& height);
};