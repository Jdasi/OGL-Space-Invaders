#pragma once
#include "Vector2.h"

class BoundingBox
{
public:
    BoundingBox() = default;
    ~BoundingBox() = default;
    
    Vector2 getPosition() const;
    void setPosition(const Vector2 _pos);

    Vector2 getSize() const;
    void setSize(const Vector2 _size);
    
    bool contains(const BoundingBox& _other) const;

private:
    Vector2 position;
    Vector2 size;
};
