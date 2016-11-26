#pragma once
#include "SpriteObject.h"
#include "TextObject.h"
#include "CollisionTypes.h"

class CollisionManager;

// Abstract class to allow for the creation of game sprites and text objects
// and decouple Game from individual states.
class ObjectFactory
{
public:
    ObjectFactory()
        : collision_manager(nullptr)
    {
    }

    virtual ~ObjectFactory() = default;

    virtual std::unique_ptr<SpriteObject> createSprite(const std::string& _texture, 
        const Vector2 _pos, const CollisionType _collision_type = CollisionType::NONE) = 0;

    virtual std::unique_ptr<TextObject> createText(const std::string& _str, 
        const Vector2 _pos, const float _size, const float _colour[3]) = 0;

    void linkCollisionManager(CollisionManager* _collision_manager)
    {
        collision_manager = _collision_manager;
    }

protected:
    CollisionManager* collision_manager;
};
