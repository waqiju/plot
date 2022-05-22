﻿#pragma once
#include "camera.h"
#include "geometry/geometry.h"
#include "entity/ec.h"
#include "mesh.h"


class SpaceGrid
{
public:
    static const float kTicks[22];
    static const int kTicksLength;

    SpaceGrid(Camera* camera, Transform* transform);
    void Render();
    void RenderLine();
    void RenderLabel();

private:

    Camera* m_Camera;
    // 世界坐标下的相机左下角和右上角
    Vector3 m_LeftBottom;
    Vector3 m_RightTop;
    Transform* m_Transform;
    Mesh* m_Mesh;

    void CalculateInterval(int pixel, int& tickGradeX, int& tickGradeY);
    float GetTick(int index);
};