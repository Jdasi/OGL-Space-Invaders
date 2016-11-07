#pragma once

class Vector2
{
public:
    Vector2()
        : Vector2(0, 0)
    {
    }

    Vector2(unsigned int _x, unsigned int _y)
        : x(_x)
        , y(_y)
    {
    }

    unsigned int x;
    unsigned int y;
};
