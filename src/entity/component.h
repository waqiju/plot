#pragma once
#include <vector>
#include "entity/world_object.h"

class Entity;

class Component: public WorldObject
{
public:
    Component(Entity* owner);
    Entity* OnwerEntity();
    bool Enabled();

private:
    void Enable(bool status);

private:
    Entity* m_OwerEntity = NULL;
    bool m_Enabled = true;
};