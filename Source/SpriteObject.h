#pragma once
#include <string>

#include <Engine/Sprite.h>

#include "Renderable.h"

class SpriteObject : public Renderable
{
public:
    SpriteObject(const std::shared_ptr<ASGE::Renderer>& _renderer, 
        ObjectRendererDeleter& _object_renderer_deleter, const std::string& _texture, 
        const Vector2 _pos);
    virtual ~SpriteObject() = default;

    Vector2 getPosition() const override;
    void setPosition(const Vector2 _pos) override;
    void modifyPosition(int _x, int _y) override;

    Vector2 getSize() const;
    void setSize(const Vector2 _size);

    float getScale() const;
    void setScale(float _scale);

    bool collisionTest(const SpriteObject& _other) const;

private:
    void render() override;

    std::unique_ptr<ASGE::Sprite> sprite;
};
