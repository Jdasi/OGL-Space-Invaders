#include "SpriteObject.h"

SpriteObject::SpriteObject(const std::shared_ptr<ASGE::Renderer>& renderer, const std::string& texture, const Position position)
    : Renderable(renderer)
{
    m_sprite = renderer->createSprite();
    m_sprite->position[0] = position.x;
    m_sprite->position[1] = position.y;

    if (!m_sprite->loadTexture(texture.c_str()))
    {
        throw std::runtime_error("Error loading sprite texture.");
    }
}



void SpriteObject::setPosition(const Position position)
{
    m_sprite->position[0] = position.x;
    m_sprite->position[1] = position.y;
}



Position SpriteObject::getPosition() const
{
    return { m_sprite->position[0], m_sprite->position[1] };
}



void SpriteObject::render()
{
    if (m_visible)
    {
        m_sprite->render(m_renderer);
    }
}


