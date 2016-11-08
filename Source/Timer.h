#pragma once
#include <chrono>

class Timer
{
public:
    Timer()
    {
        reset();
    }

    ~Timer() = default;

    void reset()
    {
        start = std::chrono::system_clock::now();
    }

    float get_time_difference() const
    {
        auto now = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_seconds = now - start;

        return elapsed_seconds.count();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start;
};
