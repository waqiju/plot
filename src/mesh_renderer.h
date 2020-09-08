#pragma once
#include "mesh.h"
#include "material.h"
#include "entity/component.h"


class MeshRenderer : public Component
{
public:
    MeshRenderer(Entity* owner);
public:
    Material* material = NULL;
    Mesh* mesh = NULL;
public:
    void Render();
};