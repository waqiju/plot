#pragma once
#include "entity/ec.h"
#include "camera_helper.h"


class StockVerticalLayout: public Component
{
public:
    static float OnePixelSize;
    static void UpdateOnePixelSize();

    StockVerticalLayout(Entity* owner);
    void ApplyLayout();

    float spacing = 1;
    CameraHelper::UnitOfLength unit = CameraHelper::UnitOfLength::World;
};