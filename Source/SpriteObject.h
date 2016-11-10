#pragma once
#include <string>
#include <functional>

#include <Engine/Sprite.h>

#include "Renderable.h"

class SpriteObject : public Renderable
{
public:
    SpriteObject(const std::shared_ptr<ASGE::Renderer>& _renderer,
        std::function<void(Renderable*)> _delete_render_object,
        const std::string& _texture, const Vector2 _pos);
    virtual ~SpriteObject() = default;

    Vector2 getSize() const;
    void setSize(const Vector2 _size);

    float getScale() const;
    void setScale(float _scale);

    bool collisionTest(const SpriteObject& _other) const;

private:
    void render() override;

    std::unique_ptr<ASGE::Sprite> sprite;
};
