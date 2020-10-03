#include "transform.h"

void Transform::SetTrsMatrix(const Matrix4x4& inMatrix)
{
	inMatrix.Decompose(m_LocalPosition, m_LocalRotation, m_LocalScale);
}
