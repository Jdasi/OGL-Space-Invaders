#include "CollisionManager.h"
#include "CollisionTypes.h"
#include "SpriteObject.h"
#include "StateHandler.h"

CollisionManager::CollisionManager(
    std::function<bool(SpriteObject*, SpriteObject*)> _on_collision_event)
    : on_collision_event(_on_collision_event)
{
    collision_objects.reserve(100);
}



void CollisionManager::tick() const
{
    testForCollisions();
}



void CollisionManager::addCollisionObject(SpriteObject* _object)
{
    // Pass collision object a function so it can delete itself from collision_objects.
    _object->registerDeleteEvent([_object, this]()
        {
            collision_objects.erase(std::remove_if(
                collision_objects.begin(), 
                collision_objects.end(), 
                [_object](SpriteObject* spr){ return spr == _object; }), 
                collision_objects.end());
        });

    collision_objects.push_back(_object);
}



void CollisionManager::testForCollisions() const
{
    for (auto& object : collision_objects)
    {
        // Prune out objects that are never going to collide with each other.
        CollisionType object_collision_type = object->getCollisionType();
        if (object_collision_type == CollisionType::BARRIER ||
            object_collision_type == CollisionType::SHIP ||
            object_collision_type == CollisionType::ALIEN)
        {
            continue;
        }

        if (!object->isVisible())
        {
            continue;
        }

        for (auto& other : collision_objects)
        {
            if (object == other)
            {
                continue;
            }

            if (object->getCollisionType() == other->getCollisionType())
            {
                continue;
            }

            if (!other->isVisible())
            {
                continue;
            }

            if (collisionTest(object, other))
            {
                if (on_collision_event(object, other))
                {
                    // Something was deleted, stop iterating.
                    return;
                }
            }
        }
    }
}



bool CollisionManager::collisionTest(SpriteObject* _object, SpriteObject* _other) const
{
    Vector2 object_pos = _object->getPosition();
    Vector2 other_pos = _other->getPosition();

    Vector2 object_size = _object->getSize();
    Vector2 other_size = _other->getSize();

    if (object_pos.x + object_size.x >= other_pos.x &&
        object_pos.x <= (other_pos.x + other_size.x) &&
        object_pos.y + object_size.y >= other_pos.y &&
        object_pos.y <= (other_pos.y + other_size.y))
    {
        return true;
    }

    return false;
}


