#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(std::vector<std::unique_ptr<SpriteObject>> _sprites)
    : sprites(std::move(_sprites))
    , animation_frame(0)
{
    setAnimationFrame(0);
}



void AnimatedSprite::setVisible(bool _b)
{
    if (_b)
    {
        setAnimationFrame(animation_frame);
    }
    else
    {
        hideAllSprites();
    }
}



int AnimatedSprite::getAnimationFrame() const
{
    return animation_frame;
}



void AnimatedSprite::setAnimationFrame(const int _frame)
{
    hideAllSprites();
    sprites[_frame]->setVisible(true);
    animation_frame = _frame;
}



void AnimatedSprite::setNextAnimationFrame()
{
    ++animation_frame;
    if (animation_frame >= sprites.size())
    {
        animation_frame = 0;
    }

    setAnimationFrame(animation_frame);
}



Vector2 AnimatedSprite::getSize() const
{
    return sprites.at(animation_frame)->getSize();
}



void AnimatedSprite::setSize(const Vector2 _size) const
{
    for (auto& sprite : sprites)
    {
        sprite->setSize(_size);
    }
}



Vector2 AnimatedSprite::getPosition() const
{
    return sprites.at(0)->getPosition();
}



void AnimatedSprite::setPosition(const Vector2 _pos) const
{
    for (auto& sprite : sprites)
    {
        sprite->setPosition(_pos);
    }
}



void AnimatedSprite::modifyPosition(const Vector2 _pos) const
{
    for (auto& sprite : sprites)
    {
        sprite->modifyPosition(_pos);
    }
}



bool AnimatedSprite::ownsSpriteObject(SpriteObject* _object) const
{
    for (auto& sprite : sprites)
    {
        if (sprite.get() == _object)
        {
            return true;
        }
    }

    return false;
}



void AnimatedSprite::hideAllSprites() const
{
    for (auto& sprite : sprites)
    {
        sprite->setVisible(false);
    }
}


