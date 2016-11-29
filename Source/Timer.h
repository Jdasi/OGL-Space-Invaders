#pragma once
#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer() = default;

    void reset();
    float getTimeDifference() const;

private:
    std::chrono::time_point<std::chrono::system_clock> start;
};
