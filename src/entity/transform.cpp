#include "transform.h"
#include <algorithm>
#include "entity/entity.h"
#include "entity/world.h"


void Transform::SetTrsMatrix(const Matrix4x4& inMatrix)
{
	inMatrix.Decompose(m_LocalPosition, m_LocalRotation, m_LocalScale);
	MarkAsDirty();
}

void Transform::SetPosition(const Vector3& position)
{
	if (m_Parent == NULL)
	{
		m_LocalPosition = position;
	}
	else
	{
		m_LocalPosition = m_Parent->WorldToLocalMatrix().MultiplyPoint3x4(position);
	}
	MarkAsDirty();
}

void Transform::SetParent(Transform* parent)
{
	if (parent == m_Parent)
		return;

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
		// 从 World 的根结点中移除
		m_OwerEntity->OnwerWorld()->RemoveFromEntities(m_OwerEntity);
	}

	m_Parent = parent;
	if (parent != NULL)
	{
		parent->m_Children.push_back(this);
	}
	else
	{
		// 加入 World 根结点
		m_OwerEntity->OnwerWorld()->AttachEntity(m_OwerEntity);
	}
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
			child->onChanged();
		}
	}
}

void Transform::MarkAsDirty(bool flush)
{
	m_IsTrsDirty = true;
	Flush(true, true);
	// event
	onChanged();
}

Transform::~Transform() 
{ 
	Destroy();

	for (auto child : m_Children)
	{
		delete child->m_OwerEntity;
	}
}

void Transform::Destroy()
{
	if (InDestroy())
		return;
	SetFlagDestroyStart();

	// self
	if (m_Parent != NULL)
	{
		if (!m_Parent->InDestroy())
		{
			std::vector<Transform*>& list = m_Parent->m_Children;
			auto iterator = std::find(list.begin(), list.end(), this);
			if (iterator != list.end())
			{
				list.erase(iterator);
			}
		}
	}
	else
	{
		// 从 World 的根结点中移除
		m_OwerEntity->OnwerWorld()->RemoveFromEntities(m_OwerEntity);
	}
	Component::Destroy();

	// children
	for (auto child : m_Children)
	{
		child->m_OwerEntity->Destroy();
	}
	SetFlagDestroyEnd();
}

void Transform::Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader)
{
    auto parentId = PrefabLoader::GetIntMember(pbComponentObj, "parent");
    this->SetParent(PrefabLoader::FindObject<Transform>(parentId));
    int vectorID = PrefabLoader::GetIntMember(pbComponentObj, "localPosition");
    if (vectorID)
    {
        ConvertVector3(loader.GetObject(vectorID), m_LocalPosition, loader.Prefab());
        this->MarkAsDirty();
    }
}