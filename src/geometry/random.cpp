#include "random.h"
#include <cstdlib>
#include <ctime>


Random Random::s_Random;

Random::Random()
{
    srand(time(NULL));
}

float Random::Next()
{
    return rand() / static_cast<float>(RAND_MAX);
}

int Random::NextInt()
{
    return rand();
}

float Random::Range(float min, float max)
{
    return min + (max - min) * Next();
}