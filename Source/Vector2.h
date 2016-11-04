#pragma once

class Vector2
{
public:
    Vector2()
        : Vector2(0, 0)
    {
    }

    Vector2(const float x, const float y)
        : x(x)
        , y(y)
    {
    }

    Vector2 operator+(const Vector2& v) const
    {
        return Vector2({ x + v.x, y + v.y });
    }

    float x = 0;
    float y = 0;
};
