#include "component.h"
#include "entity/entity.h"


Component::Component(Entity* owner)
{
    m_OwerEntity = owner;
}

Entity* Component::OnwerEntity()
{
    return m_OwerEntity;
}

Transform* Component::GetTransform()
{
    return m_OwerEntity->GetTransform();
}

bool Component::Enabled()
{
    return m_Enabled;
}

void Component::Enable(bool status)
{
    m_Enabled = status;
}
