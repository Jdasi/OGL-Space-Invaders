#include "SpriteObject.h"
#include "CollisionTypes.h"

SpriteObject::SpriteObject(const std::shared_ptr<ASGE::Renderer>& _renderer, 
    std::function<void(Renderable*)> _delete_render_object, const std::string& _texture,
    const Vector2 _pos, const CollisionType _collision_type, float _lifetime)
    : Renderable(_renderer, _delete_render_object, _pos)
    , collision_type(_collision_type)
    , lifetime(_lifetime)
{
    sprite = renderer->createSprite();

    if (!sprite->loadTexture(_texture.c_str()))
    {
        throw std::runtime_error("Error loading sprite texture.");
    }
}



SpriteObject::~SpriteObject()
{
    for (auto& event: on_delete_events)
    {
        event();
    }
}



Vector2 SpriteObject::getSize() const
{
    return { sprite->size[0] * sprite->scale, sprite->size[1] * sprite->scale };
}



void SpriteObject::setSize(const Vector2 _size)
{
    sprite->size[0] = static_cast<int>(_size.x);
    sprite->size[1] = static_cast<int>(_size.y);
}



float SpriteObject::getScale() const
{
    return sprite->scale;
}



void SpriteObject::setScale(float _scale) const
{
    sprite->scale = _scale;
}



CollisionType SpriteObject::getCollisionType() const
{
    return collision_type;
}



BoundingBox SpriteObject::getBoundingBox() const
{
    return bounding_box;
}



void SpriteObject::registerDeleteEvent(std::function<void()> _event)
{
    on_delete_events.push_back(_event);
}



void SpriteObject::tick(float _dt)
{
    lifetime -= _dt;
}



bool SpriteObject::expired() const
{
    return lifetime < 0;
}



void SpriteObject::render()
{
    if (visible)
    {
        sprite->position[0] = static_cast<int>(position.x);
        sprite->position[1] = static_cast<int>(position.y);

        bounding_box.setPosition(position);
        bounding_box.setSize({ static_cast<float>(sprite->size[0]), 
            static_cast<float>(sprite->size[1]) });

        sprite->render(renderer);
    }
}


