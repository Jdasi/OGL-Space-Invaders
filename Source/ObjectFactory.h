#pragma once
#include "SpriteObject.h"
#include "TextObject.h"

// Abstract class to allow for the creation of game sprites and text objects
// and decouple Game from individual states.
class ObjectFactory
{
public:
    ObjectFactory() = default;
    virtual ~ObjectFactory() = default;

    virtual std::unique_ptr<SpriteObject> createSprite(const std::string& _texture, 
        const Vector2 _pos) = 0;

    virtual std::unique_ptr<TextObject> createText(const std::string& _str, 
        const Vector2 _pos, const float _size, const float _colour[3]) = 0;
};
