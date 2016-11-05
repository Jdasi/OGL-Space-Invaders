#include "TextObject.h"

TextObject::TextObject(const std::shared_ptr<ASGE::Renderer>& renderer)
    : Renderable(renderer)
    , m_position()
    , m_scale(0.0f)
    , m_rgb(ASGE::COLOURS::WHITE)
{
}



TextObject::TextObject(const std::shared_ptr<ASGE::Renderer>& renderer,
                       const std::string& str, const Vector2 position, const float scale,
                       const float colour[3])
    : Renderable(renderer)
    , m_position(position)
    , m_str(str)
    , m_scale(scale)
    , m_rgb(colour)
{
}



std::string TextObject::getString() const
{
    return m_str;
}



void TextObject::setString(const std::string& str)
{
    m_str = str;
}



Vector2 TextObject::getPosition() const
{
    return m_position;
}



void TextObject::setPosition(const Vector2 position)
{
    m_position = position;
}



void TextObject::modifyPosition(int x, int y)
{
    m_position.x += x;
    m_position.y += y;
}



float TextObject::getScale() const
{
    return m_scale;
}



void TextObject::setScale(float f)
{
    m_scale = f;
}



ASGE::Colour TextObject::getColour() const
{
    return m_rgb;
}



void TextObject::setColour(const float colour[3])
{
    m_rgb = colour;
}



void TextObject::render()
{
    if (m_visible)
    {
        m_renderer->renderText(m_str.c_str(), m_position.x, m_position.y, m_scale, m_rgb);
    }
}


