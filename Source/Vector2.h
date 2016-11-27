#pragma once

struct Vector2
{
    Vector2()
        : Vector2(0, 0)
    {
    }

    Vector2(float _x, float _y)
        : x(_x)
        , y(_y)
    {
    }

    bool operator==(const Vector2& v) const
    {
        if (v.x == x && v.y == y)
        {
            return true;
        }

        return false;
    }

    float x;
    float y;
};
