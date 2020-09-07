#pragma once
#include "mesh.h"
#include "material.h"


class MeshRenderer
{
public:
    Material* material = NULL;
    Mesh* mesh = NULL;
public:
    void Render();
};