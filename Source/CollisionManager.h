#pragma once
#include <vector>
#include <functional>

class StateHandler;
class SpriteObject;

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
    bool collisionTest(SpriteObject* _object, SpriteObject* _other) const;

    std::function<bool(SpriteObject*, SpriteObject*)> on_collision_event;
    std::vector<SpriteObject*> collision_objects;
};