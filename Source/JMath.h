#pragma once

namespace JMath
{
    template <typename T>
    T clamp(T _original, T _min, T _max)
    {
        if (_original < _min)
        {
            return _min;
        }

        if (_original > _max)
        {
            return _max;
        }

        return _original;
    }
}
