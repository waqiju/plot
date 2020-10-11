#pragma once


class Random
{
public:
    static float Next();
    static int NextInt();
    static float Range(float min, float max);

private:
    Random();
    static Random s_Random;
};