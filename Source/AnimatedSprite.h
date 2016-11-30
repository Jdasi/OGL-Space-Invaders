#pragma once
#include <vector>
#include <memory>

#include "SpriteObject.h"

/* A special class that contains a vector of SpriteObjects for the purposes of 
 * creating basic sprite animation.
 * 
 * This is done by tracking the current animation frame and showing only that one
 * while hiding the rest.
 */
class AnimatedSprite
{
public:
    AnimatedSprite(std::vector<std::unique_ptr<SpriteObject>> _sprites);
    virtual ~AnimatedSprite() = default;

    void setVisible(bool _b);

    int getAnimationFrame() const;
    void setAnimationFrame(const int _frame);
    void setNextAnimationFrame();

    Vector2 getSize() const;
    void setSize(const Vector2 _size) const;

    Vector2 getPosition() const;
    void setPosition(const Vector2 _pos) const;
    void modifyPosition(const Vector2 _pos) const;
    
    bool ownsSpriteObject(SpriteObject* _object) const;

private:
    void hideAllSprites() const;

    std::vector<std::unique_ptr<SpriteObject>> sprites;
    unsigned int animation_frame;
};
