#pragma once
#include "Vector2.h"

/* A "Rectangle" that SpriteObjects use to determine their collision area.
 * The CollisionManager uses the BoundingBox to detect collisions in the Gameplay State.
 */
class BoundingBox
{
public:
    BoundingBox() = default;
    ~BoundingBox() = default;
    
    Vector2 getPosition() const;
    void setPosition(const Vector2 _pos);

    Vector2 getSize() const;
    void setSize(const Vector2 _size);
    
    bool overlaps(const BoundingBox& _other) const;

private:
    Vector2 position;
    Vector2 size;
};
