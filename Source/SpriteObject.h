#pragma once
#include <string>
#include <functional>
#include <vector>

#include <Engine/Sprite.h>

#include "Renderable.h"

enum class CollisionType;

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

    void registerDeleteEvent(std::function<void()> _event);

    void tick(float _dt);
    bool expired() const;

private:
    void render() override;

    std::unique_ptr<ASGE::Sprite> sprite;
    std::vector<std::function<void()>> on_delete_events;
    CollisionType collision_type;
    float lifetime;
};
