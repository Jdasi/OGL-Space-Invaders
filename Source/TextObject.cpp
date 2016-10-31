#include "TextObject.h"

TextObject::TextObject(const std::shared_ptr<ASGE::Renderer>& renderer)
    : Renderable(renderer)
    , m_position()
    , m_pos({ 0, 0 })
    , m_scale(0.0f)
    , m_rgb(ASGE::COLOURS::WHITE)
{
}



TextObject::TextObject(const std::shared_ptr<ASGE::Renderer>& renderer, const std::string& s, const Position position, const float size, const float colour[3])
    : Renderable(renderer)
    , m_position(position)
    , m_str(s)
    , m_scale(size)
    , m_rgb(colour)
{
}



void TextObject::setString(const std::string& s)
{
    m_str = s;
}



std::string TextObject::getString() const
{
    return m_str;
}



void TextObject::setScale(float f)
{
    m_scale = f;
}



float TextObject::getScale() const
{
    return m_scale;
}



void TextObject::setColour(const float colour[3])
{
    m_rgb = colour;
}



ASGE::Colour TextObject::getColour() const
{
    return m_rgb;
}



void TextObject::setPosition(const Position position)
{
    m_position = position;
}



Position TextObject::getPosition() const
{
    return m_position;
}



void TextObject::render()
{
    if (m_visible)
    {
        m_renderer->renderText(m_str.c_str(), m_position.x, m_position.y, m_scale, m_rgb);
    }
}


