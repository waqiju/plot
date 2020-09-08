#include "component.h"

Component::Component(Entity* owner)
{
    m_OwerEntity = owner;
}

Entity* Component::OnwerEntity()
{
    return m_OwerEntity;
}

bool Component::Enabled()
{
    return m_Enabled;
}

void Component::Enable(bool status)
{
    m_Enabled = status;
}
