#include "transform.h"

void Transform::SetTrsMatrix(const Matrix4x4& inMatrix)
{
	inMatrix.Decompose(m_LocalPosition, m_LocalRotation, m_LocalScale);
	m_IsTrsDirty = true;
}

void Transform::Flush(bool includeChildren, bool force)
{
	if (m_IsTrsDirty || force)
	{
		if (m_Parent == NULL)
		{
			m_LocalToWorld = Matrix4x4::TRS(m_LocalPosition, m_LocalRotation, m_LocalScale);
		}
		else
		{
			m_LocalToWorld = m_Parent->LocalToWorldMatrix() * Matrix4x4::TRS(m_LocalPosition, m_LocalRotation, m_LocalScale);
		}
		force = true;
	}

	if (includeChildren)
	{
		for (auto child : m_Children)
		{
			child->Flush(includeChildren, force);
		}
	}
}
