#include "TextObject.h"
#include "ObjectRenderer.h"

TextObject::TextObject(const std::shared_ptr<ASGE::Renderer>& _renderer, ObjectRendererDeleter& _object_renderer_deleter,
                       const std::string& _str, const Vector2 _pos, const float _scale,
                       const float _colour[3])
    : Renderable(_renderer, _object_renderer_deleter)
    , position(_pos)
    , str(_str)
    , scale(_scale)
    , rgb(_colour)
{
}



std::string TextObject::getString() const
{
    return str;
}



void TextObject::setString(const std::string& _str)
{
    str = _str;
}



Vector2 TextObject::getPosition() const
{
    return position;
}



void TextObject::setPosition(const Vector2 _pos)
{
    position = _pos;
}



void TextObject::modifyPosition(int _x, int _y)
{
    position.x += _x;
    position.y += _y;
}



float TextObject::getScale() const
{
    return scale;
}



void TextObject::setScale(float _scale)
{
    scale = _scale;
}



ASGE::Colour TextObject::getColour() const
{
    return rgb;
}



void TextObject::setColour(const float _colour[3])
{
    rgb = _colour;
}



void TextObject::render()
{
    if (visible)
    {
        renderer->renderText(str.c_str(), position.x, position.y, scale, rgb);
    }
}


