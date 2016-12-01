#pragma once
#include <functional>
#include <vector>

#include <Engine/Sprite.h>

#include "Renderable.h"
#include "BoundingBox.h"

enum class CollisionType;

/* Derived from Renderable. Has ownership of an ASGE::Sprite.
 * SpriteObjects are naturally game-oriented, so they maintain a list of on-delete-events
 * that will be called when the SpriteObject is destroyed.
 * 
 * SpriteObjects can also have a lifetime so long as something is there to pick up when
 * the SpriteObject has expired.
 */
class SpriteObject : public Renderable
{
public:
    SpriteObject(const std::shared_ptr<ASGE::Renderer>& _renderer,
        std::function<void(Renderable*)> _delete_render_object,
        const std::string& _texture, const Vector2 _pos,
        const CollisionType _collision_type, float _lifetime);
    virtual ~SpriteObject();

    Vector2 getSize() const;
    void setSize(const Vector2 _size);

    float getScale() const;
    void setScale(float _scale) const;

    CollisionType getCollisionType() const;
    BoundingBox getBoundingBox() const;

    void registerDeleteEvent(std::function<void()> _event);

    void tick(float _dt);
    bool expired() const;

private:
    void render() override;

    std::unique_ptr<ASGE::Sprite> sprite;
    std::vector<std::function<void()>> on_delete_events;
    CollisionType collision_type;
    BoundingBox bounding_box;
    float lifetime;
};
