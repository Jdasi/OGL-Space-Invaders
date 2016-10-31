#pragma once
#include <string>

#include <Engine/Sprite.h>

#include "Renderable.h"

class SpriteObject : public Renderable
{
public:
    SpriteObject(const std::shared_ptr<ASGE::Renderer>& renderer, 
                 const std::string& texture, const Position position);
    virtual ~SpriteObject() = default;

    void setPosition(const Position position) override;
    Position getPosition() const override;

private:
    void render() override;

    std::unique_ptr<ASGE::Sprite> m_sprite;
};
