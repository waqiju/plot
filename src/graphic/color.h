#pragma once
#include <string>


class Color
{
public:
    // constant
    static const Color white;
    static const Color black;
    static const Color red;
    static const Color green;
    static const Color blue;

    Color(float inR, float inG, float inB) : r(inR), g(inG), b(inB), a(1.0f) {}
    Color(float inR, float inG, float inB, float inA) : r(inR), g(inG), b(inB), a(inA) {}
    std::string ToString() const;

    float r, g, b, a;
};