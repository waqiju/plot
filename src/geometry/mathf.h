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
    static float IsZero(float x)
    {
        return Equal(x, 0, Epsilon());
    }
    static float Equal(float x, float y, float maxULP)
    {
        return ::std::abs(x - y) <= maxULP;
    }
    static float Clamp(float x, float minValue, float maxValue)
    {
        return ::std::min(std::max(x, minValue), maxValue);
    }
};