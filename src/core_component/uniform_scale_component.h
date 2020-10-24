/*
 * 有时候，模型的顶点是对称的，但是在从 Model 到 World 到 Viewport 的空间转换过程中，包含有非对称的缩放。这样在Viewport中，模型从屏幕空间上看起来，跟在模型空间看起来，比例发生了变化。
 * 很多时候，出于美观的角度，希望在 Viewport 中保留和 Model 中一样的比例。
 * UniformScaleMode 就是用来实现这样的需求，已知中心点和缩放的主轴。
 */
#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"


enum class UniformScaleMode
{
    None,
    FixedWidthInWorldSpace,
};


class UniformScaleComponent: public Component
{
public:
    UniformScaleComponent(Entity* owner);

    Component* target;
    UniformScaleMode mode;

    void ResizeBounds();
};