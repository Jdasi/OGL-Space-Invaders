#include "SpriteObject.h"

SpriteObject::SpriteObject(const std::shared_ptr<ASGE::Renderer>& _renderer, 
    std::function<void(Renderable*)> _delete_render_object, const std::string& _texture,
    const Vector2 _pos)
    : Renderable(_renderer, _delete_render_object)
{
    sprite = renderer->createSprite();
    sprite->position[0] = _pos.x;
    sprite->position[1] = _pos.y;

    if (!sprite->loadTexture(_texture.c_str()))
    {
        throw std::runtime_error("Error loading sprite texture.");
    }
}



Vector2 SpriteObject::getPosition() const
{
    return { sprite->position[0], sprite->position[1] };
}



void SpriteObject::setPosition(const Vector2 _pos)
{
    sprite->position[0] = _pos.x;
    sprite->position[1] = _pos.y;
}



void SpriteObject::modifyPosition(int _x, int _y)
{
    sprite->position[0] += _x;
    sprite->position[1] += _y;
}



Vector2 SpriteObject::getSize() const
{
    int scale = static_cast<int>(sprite->scale);
    return { sprite->size[0] * scale, sprite->size[1] * scale };
}



void SpriteObject::setSize(const Vector2 _size)
{
    sprite->size[0] = _size.x;
    sprite->size[1] = _size.y;
}



float SpriteObject::getScale() const
{
    return sprite->scale;
}



void SpriteObject::setScale(float _scale)
{
    sprite->scale = _scale;
}



bool SpriteObject::collisionTest(const SpriteObject& _other) const
{
    if (sprite->position[0] + sprite->size[0] >= _other.getPosition().x &&
        sprite->position[0] <= (_other.getPosition().x + _other.getSize().x) &&
        sprite->position[1] + sprite->size[1] >= _other.getPosition().y &&
        sprite->position[1] <= (_other.getPosition().y + _other.getSize().y))
    {
        return true;
    }

    return false;
}



void SpriteObject::render()
{
    if (visible)
    {
        sprite->render(renderer);
    }
}


