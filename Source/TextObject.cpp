#include "TextObject.h"
#include "Constants.h"

TextObject::TextObject(const std::shared_ptr<ASGE::Renderer>& _renderer, 
    std::function<void(Renderable*)> _delete_render_object, const std::string& _str,
    const Vector2 _pos, const float _scale, const float _colour[3])
    : Renderable(_renderer, _delete_render_object, _pos)
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
        int pos_x = static_cast<int>(position.x);
        int pos_y = static_cast<int>(position.y);

        renderer->renderText(str.c_str(), pos_x, pos_y, scale, rgb);
    }
}


