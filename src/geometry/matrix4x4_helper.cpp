#include "matrix4x4_helper.h"


namespace Matrix4x4Helper
{
    void ApplyMatrixForEach(const Matrix4x4& matrix, std::vector<Vector3>& vertices, int begin, int end)
    {
        for (int i = begin; i < end; ++i)
        {
            vertices[i] = matrix.MultiplyPoint3x4(vertices[i]);
        }
    }
}