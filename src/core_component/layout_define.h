#pragma once


enum class LayoutAlignment
{
    Left = 0,
    Center = 1,
    Right = 2,
    Top = 4,
    Middle = 8,
    Bottom = 16,
    // 复合
    CenterMiddle = Center | Middle,  // 9
};