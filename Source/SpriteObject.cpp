#include "SpriteObject.h"

SpriteObject::SpriteObject(const std::shared_ptr<ASGE::Renderer>& _renderer, 
    std::function<void(Renderable*)> _delete_render_object, const std::string& _texture,
    const Vector2 _pos)
    : Renderable(_renderer, _delete_render_object, _pos)
{
    sprite = renderer->createSprite();
    sprite->position[0] = static_cast<unsigned int>(_pos.x);
    sprite->position[1] = static_cast<unsigned int>(_pos.y);

    if (!sprite->loadTexture(_texture.c_str()))
    {
        throw std::runtime_error("Error loading sprite texture.");
    }
}



Vector2 SpriteObject::getSize() const
{
    return { sprite->size[0] * sprite->scale, sprite->size[1] * sprite->scale };
}



void SpriteObject::setSize(const Vector2 _size)
{
    sprite->size[0] = static_cast<unsigned int>(_size.x);
    sprite->size[1] = static_cast<unsigned int>(_size.y);
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
    if (position.x + sprite->size[0] >= _other.getPosition().x &&
        position.x <= (_other.getPosition().x + _other.getSize().x) &&
        position.y + sprite->size[1] >= _other.getPosition().y &&
        position.y <= (_other.getPosition().y + _other.getSize().y))
    {
        return true;
    }

    return false;
}



void SpriteObject::render()
{
    if (visible)
    {
        sprite->position[0] = static_cast<unsigned int>(position.x);
        sprite->position[1] = static_cast<unsigned int>(position.y);

        sprite->render(renderer);
    }
}


