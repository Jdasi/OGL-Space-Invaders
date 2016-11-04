#include "SpriteObject.h"

SpriteObject::SpriteObject(const std::shared_ptr<ASGE::Renderer>& renderer, const std::string& texture, const Vector2 position)
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



Vector2 SpriteObject::getPosition() const
{
    return { m_sprite->position[0], m_sprite->position[1] };
}



void SpriteObject::setPosition(const Vector2 position)
{
    m_sprite->position[0] = position.x;
    m_sprite->position[1] = position.y;
}



void SpriteObject::modifyPosition(const Vector2 position)
{
    m_sprite->position[0] += position.x;
    m_sprite->position[1] += position.y;
}



Vector2 SpriteObject::getSize() const
{
    return { m_sprite->size[0], m_sprite->size[1] };
}



void SpriteObject::setSize(const Vector2 size)
{
    m_sprite->size[0] = size.x;
    m_sprite->size[1] = size.y;
}



void SpriteObject::render()
{
    if (m_visible)
    {
        m_sprite->render(m_renderer);
    }
}


