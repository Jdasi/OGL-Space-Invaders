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
        m_start = std::chrono::system_clock::now();
    }

    float get_time_difference() const
    {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_seconds = now - m_start;

        return elapsed_seconds.count();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_start;
};
