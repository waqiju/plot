#pragma once


class StockLayoutItem: public Component
{
public:
    StockLayoutItem(Entity* owner);

    int priority = 0;
};