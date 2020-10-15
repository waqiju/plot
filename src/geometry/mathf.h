#pragma once
#include <utility>


class Mathf
{
public:
    static constexpr float PI = 3.14159274F;
    static constexpr float Deg2Rad = 0.0174532924F;
    static constexpr float Rad2Deg = 57.29578F;

    static float Epsilon()
    {
        return ::std::numeric_limits<float>::epsilon();
    }
    static bool IsZero(float x)
    {
        return Equal(x, 0, Epsilon());
    }
    static bool Equal(float x, float y)
    {
        return ::std::abs(x - y) <= Epsilon();
    }
    static bool Equal(float x, float y, float maxULP)
    {
        return ::std::abs(x - y) <= maxULP;
    }
    static float Clamp(float x, float minValue, float maxValue)
    {
        return ::std::min(std::max(x, minValue), maxValue);
    }
    static int Ceil(float x)
    {
        return static_cast<int>(std::ceil(x));
    }
    static int Floor(float x)
    {
        return static_cast<int>(std::floor(x));
    }
    static float ToCeilUnit(float x, float unit, float maxULP)
    {
        int multiply = static_cast<int>(x / unit);
        float x1 = unit * multiply;
        if (Equal(x, x1, maxULP))
            return x1;
        return x1 + unit;
    }
    static float ToFloorUnit(float x, float unit, float maxULP)
    {
        int multiply = static_cast<int>(x / unit);
        float x1 = unit * multiply;
        if (Equal(x, x1, maxULP))
            return x1;
        return x1 - unit;
    }
    static float ToNearUnit(float x, float unit, float maxULP)
    {
        if (x > 0)
            return ToCeilUnit(x, unit, maxULP);
        else
            return ToFloorUnit(x, unit, maxULP);
    }

    static float Max(float x, float y)
    {
        return std::max(x, y);
    }

    static float Min(float x, float y)
    {
        return std::min(x, y);
    }
};