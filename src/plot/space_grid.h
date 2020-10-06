#pragma once
#include "camera.h"
#include "geometry/geometry.h"
#include "entity/ec.h"
#include "mesh.h"


class SpaceGrid
{
public:
    SpaceGrid(Camera* camera);
    void Render();

private:
    Camera* m_Camera;
    // 世界坐标下的相机左下角和右上角
    Vector3 m_LeftBottom;
    Vector3 m_RightTop;
    Entity* m_Entity;
    Mesh* m_Mesh;
};