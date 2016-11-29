#include "Vector2.h"

Vector2::Vector2()
    : Vector2(0, 0)
{
}



Vector2::Vector2(float _x, float _y)
    : x(_x)
    , y(_y)
{
}



bool Vector2::operator==(const Vector2& _rhs) const
{
    if (_rhs.x == x && _rhs.y == y)
    {
        return true;
    }

    return false;
}


