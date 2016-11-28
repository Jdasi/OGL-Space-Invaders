#pragma once
#include <random>

class RandomEngine
{
public:
    RandomEngine();
    ~RandomEngine() = default;

    float randomFloat(float _min, float _max);

private:
    std::random_device rd;
    std::mt19937 mt;
};