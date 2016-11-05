#pragma once

class Vector2
{
public:
    Vector2()
        : Vector2(0, 0)
    {
    }

    Vector2(unsigned int x, unsigned int y)
        : x(x)
        , y(y)
    {
    }

    unsigned int x = 0;
    unsigned int y = 0;
};
