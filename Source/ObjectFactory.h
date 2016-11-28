#pragma once
#include "SpriteObject.h"
#include "TextObject.h"
#include "CollisionTypes.h"

class CollisionManager;

// Abstract class for ObjectRenderer to be passed around without exposing
// render specific functionality.
class ObjectFactory
{
public:
    ObjectFactory()
        : collision_manager(nullptr)
    {
    }

    virtual ~ObjectFactory() = default;

    virtual std::unique_ptr<SpriteObject> createSprite(const std::string& _texture, 
        const Vector2 _pos, const CollisionType _collision_type = CollisionType::NONE, 
        float _lifetime = 0) = 0;

    virtual std::unique_ptr<TextObject> createText(const std::string& _str, 
        const Vector2 _pos, const float _size, const float _colour[3]) = 0;

    void linkCollisionManager(CollisionManager* _collision_manager)
    {
        collision_manager = _collision_manager;
    }

protected:
    CollisionManager* collision_manager;
};
