#include "CollisionManager.h"
#include "CollisionTypes.h"
#include "SpriteObject.h"
#include "StateHandler.h"
#include "Constants.h"

CollisionManager::CollisionManager(
    std::function<bool(SpriteObject*, SpriteObject*)> _on_collision_event)
    : on_collision_event(_on_collision_event)
{
    collision_objects.reserve(COLLISION_MANAGER_RESERVE);
}



void CollisionManager::tick() const
{
    testForCollisions();
}



void CollisionManager::addCollisionObject(SpriteObject* _object)
{
    // Pass collision object a function so it can delete itself from collision_objects.
    // Lambda-ception.
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



/* Cycles through all collision objects and uses their BoundingBox to determine
 * if a collision has occured.
 * When a collision does happen, the function returns early if a change was made
 * to the vector.
 */
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
            if (object == other ||
                object->getCollisionType() == other->getCollisionType() ||
                !other->isVisible())
            {
                continue;
            }

            if (object->getBoundingBox().overlaps(other->getBoundingBox()))
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


