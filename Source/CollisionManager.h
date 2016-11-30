#pragma once
#include <vector>
#include <functional>

class StateHandler;
class SpriteObject;

/* A special manager owned by the Gameplay State who's sole purpose is to keep a vector
 * of all active collision objects, and inform the Gameplay State when a collision
 * has occurred.
 *
 * The CollisionManager uses a SpriteObject's BoundingBox to identify collisions.
 * Due to the game's simplicity, only the most mobile of collision objects are compared
 * against other collision objects in the game. This is a basic attempt at optimisation.
 */
class CollisionManager
{
public:
    explicit CollisionManager(
        std::function<bool(SpriteObject*, SpriteObject*)> _on_collision_event);
    ~CollisionManager() = default;

    void tick() const;
    void addCollisionObject(SpriteObject* _object);

private:
    void testForCollisions() const;

    std::function<bool(SpriteObject*, SpriteObject*)> on_collision_event;
    std::vector<SpriteObject*> collision_objects;
};
