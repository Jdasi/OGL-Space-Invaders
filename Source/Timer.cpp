#include "Timer.h"

Timer::Timer()
{
    reset();
}



void Timer::reset()
{
    start = std::chrono::system_clock::now();
}



float Timer::getTimeDifference() const
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_seconds = now - start;

    return elapsed_seconds.count();
}


