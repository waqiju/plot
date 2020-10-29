#pragma once
#include "entity/ec.h"


class StockLayoutItem: public Component
{
public:
    StockLayoutItem(Entity* owner);

    int priority = 0;
};