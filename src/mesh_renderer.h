#pragma once
#include "mesh.h"
#include "material.h"
#include "camera.h"
#include "entity/component.h"


class MeshRenderer : public Component
{
public:
    MeshRenderer(Entity* owner);

    Material* material = NULL;
    Mesh* mesh = NULL;
    // 如果有相机，则会在 Render 时自动传递 MVP 矩阵
    Camera* camera = NULL;

    void Render();
};