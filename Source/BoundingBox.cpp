#include "BoundingBox.h"

Vector2 BoundingBox::getPosition() const
{
    return position;
}



void BoundingBox::setPosition(const Vector2 _pos)
{
    position = _pos;
}



Vector2 BoundingBox::getSize() const
{
    return size;
}



void BoundingBox::setSize(const Vector2 _size)
{
    size = _size;
}



bool BoundingBox::contains(const BoundingBox& _other) const
{
    if (position.x + size.x >= _other.getPosition().x &&
        position.x <= (_other.getPosition().x + _other.getSize().x) &&
        position.y + size.y >= _other.getPosition().y &&
        position.y <= (_other.getPosition().y + _other.getSize().y))
    {
        return true;
    }

    return false;
}


