#pragma once
#include <string>

#include <Engine/Sprite.h>

#include "Renderable.h"

class SpriteObject : public Renderable
{
public:
    SpriteObject(const std::shared_ptr<ASGE::Renderer>& renderer, 
                 const std::string& texture, const Vector2 position);
    virtual ~SpriteObject() = default;

    Vector2 getPosition() const override;
    void setPosition(const Vector2 position) override;
    void modifyPosition(const Vector2 position) override;

    Vector2 getSize() const;
    void setSize(const Vector2 size);

private:
    void render() override;

    std::unique_ptr<ASGE::Sprite> m_sprite;
};
