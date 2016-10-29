#include "TextObject.h"

TextObject::TextObject()
    : str()
    , pos({ 0, 0 })
    , scale(0.0f)
    , rgb(ASGE::COLOURS::WHITE)
{
}

TextObject::TextObject(const std::string& s, int x, int y, float sz, const float colour[3])
    : str(s)
    , pos({ x, y })
    , scale(sz)
    , rgb(colour)
{
}

void TextObject::setString(const std::string& s)
{
    str = s;
}

std::string TextObject::getString() const
{
    return str;
}

void TextObject::setX(int x)
{
    pos[0] = x;
}

int TextObject::getX() const
{
    return pos[0];
}

void TextObject::setY(int y)
{
    pos[1] = y;
}

int TextObject::getY() const
{
    return pos[1];
}

void TextObject::setScale(float f)
{
    scale = f;
}

float TextObject::getScale() const
{
    return scale;
}

void TextObject::setColour(const float colour[3])
{
    rgb = colour;
}

ASGE::Colour TextObject::getColour() const
{
    return rgb;
}