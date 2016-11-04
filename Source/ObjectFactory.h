#pragma once
#include "SpriteObject.h"
#include "TextObject.h"

// Abstract class to allow for the creation of game sprites and text objects
// and decouple game from states.
class ObjectFactory
{
public:
    ObjectFactory() = default;
    virtual ~ObjectFactory() = default;

    virtual std::shared_ptr<SpriteObject> createSprite
        (const std::string& texture, const Vector2 position) = 0;

    virtual std::shared_ptr<TextObject> createText
        (const std::string& s, const Vector2 position, const float size, 
         const float colour[3]) = 0;
};
