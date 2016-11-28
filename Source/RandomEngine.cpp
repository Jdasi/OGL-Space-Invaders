#include "RandomEngine.h"

RandomEngine::RandomEngine()
    : mt(rd())
{
}



float RandomEngine::randomFloat(float _min, float _max)
{
    std::uniform_real_distribution<float> random(_min, _max);
    return random(mt);
}


