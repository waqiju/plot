#pragma once
#include "entity/ec.h"


class StockVerticalLayout: public Component
{
public:
    StockVerticalLayout(Entity* owner);
    void ApplyLayout();

    float spacing = 1;
};