#include "entity.h"

bool Entity::Actived()
{
    return m_Actived;
}

void Entity::SetActive(bool status)
{
    m_Actived = status;
}
