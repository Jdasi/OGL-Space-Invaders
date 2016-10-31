#pragma once

class Position
{
public:
    Position()
        : Position(0, 0)
    {
    }

    Position(const float x, const float y)
        : x(x)
        , y(y)
    {
    }

    float x;
    float y;
};
