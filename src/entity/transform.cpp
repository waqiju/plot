#include "transform.h"
#include <algorithm>
#include "entity/entity.h"
#include "entity/world.h"


void Transform::SetTrsMatrix(const Matrix4x4& inMatrix)
{
	inMatrix.Decompose(m_LocalPosition, m_LocalRotation, m_LocalScale);
	MarkAsDirty();
}

void Transform::SetParent(Transform* parent)
{
	if (m_Parent != NULL)
	{
		std::vector<Transform*>& list = m_Parent->m_Children;
		auto iterator = std::find(list.begin(), list.end(), this);
		if (iterator != list.end())
		{
			list.erase(iterator);
		}
	}
	else
	{
		// 从 World 的根节点中移除
		m_OwerEntity->OnwerWorld()->RemoveFromEntities(m_OwerEntity);
	}

	m_Parent = parent;
	parent->m_Children.push_back(this);
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

void Transform::MarkAsDirty(bool flush)
{
	m_IsTrsDirty = true;
	Flush(true, true);
}
