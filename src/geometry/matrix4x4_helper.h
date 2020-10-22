#pragma once
#include "matrix4x4.h"
#include <vector>


namespace Matrix4x4Helper
{
    void ApplyMatrixForEach(const Matrix4x4& matrix, std::vector<Vector3>& vertices, int begin, int end);
}