#include "procedural_shape.h"


// std::vector<Vector3> kTriangleVertices = {
//     Vector3(0, 0.4330127018922193f, 0),
//     Vector3(-0.5f, -0.4330127018922193f, 0),
//     Vector3(0.5f, -0.4330127018922193f, 0),
// };

int ProceduralShape::GenerateTriangle(const Vector3& v0, const Vector3& v1, std::vector<Vector3>& vertices)
{
    Vector3 center = (v0 + v1) / 2;
    // float r = Mathf::Min(v1.x - v0.x, v1.y - v0.y);
	Vector3 scaler = Vector3(v1.x - v0.x, v1.y - v0.y, 0);

    // vertices.push_back(center + kTriangleVertices[0] * scaler);
    // vertices.push_back(center + kTriangleVertices[1] * scaler);
    // vertices.push_back(center + kTriangleVertices[2] * scaler);
    return 3;
}

void ProceduralShape::AppendColors(const Color& color, int count, std::vector<Color>& colors)
{
    for (int i = 0;i < count;++i)
    {
        colors.push_back(color);
    }
}

Bounds ProceduralShape::CalculateBounds(const Matrix4x4& matrix, const Vector3& center, FlexibleShapeMode mode, float& width, float& height)
{
    // TODO 这个实现应该是在计算 biliboard, 这是这里默认是平行于 xy 平面，而非平行于 viewport
    // 暂时先这样了，目前的需求下，相机是平行于 xy 平面的
    Vector3 ruler = matrix.MultiplyPoint(Vector3::one);
    switch (mode)
    {
    case FlexibleShapeMode::FixedWidth:
    {
        height = width * ruler.x / ruler.y;
        break;
    }
    case FlexibleShapeMode::FixedHeight:
    {
        width = height * ruler.y / ruler.x;
        break;
    }
    default:
        break;
    }

    Vector3 min = Vector3(center.x - width / 2, center.y - height / 2, center.z);
    Vector3 max = Vector3(center.x + width / 2, center.y + height / 2, center.z);
    return Bounds(min, max);
}