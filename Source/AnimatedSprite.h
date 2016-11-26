#pragma once
#include <vector>
#include <memory>
#include "SpriteObject.h"

class AnimatedSprite
{
public:
    AnimatedSprite(std::vector<std::unique_ptr<SpriteObject>> _sprites);
    virtual ~AnimatedSprite() = default;

    void setVisible(bool visible);

    int getAnimationFrame() const;
    void setAnimationFrame(const int frame);
    void setNextAnimationFrame();

    Vector2 getSize() const;
    void setSize(const Vector2 _size) const;

    Vector2 getPosition() const;
    void setPosition(const Vector2 _pos) const;
    void modifyPosition(const Vector2 _pos) const;
    
    SpriteObject& getAnimationFrameSprite(const int _frame) const;

    bool containsSpriteObject(SpriteObject* _object) const;

private:
    void hideAllSprites() const;

    std::vector<std::unique_ptr<SpriteObject>> sprites;
    unsigned int animation_frame;
};
