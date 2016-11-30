#pragma once

// Simple struct to represent coordinates in 2D space.
struct Vector2
{
    Vector2();
    Vector2(float _x, float _y);

    bool operator==(const Vector2& _rhs) const;

    float x;
    float y;
};
