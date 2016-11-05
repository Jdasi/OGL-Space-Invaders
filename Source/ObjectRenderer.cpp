#include <algorithm>

#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(std::shared_ptr<ASGE::Renderer>& renderer)
    : m_renderer(renderer)
{
}



std::shared_ptr<SpriteObject> ObjectRenderer::createSprite(const std::string& texture, const Vector2 position)
{
    auto object = std::make_shared<SpriteObject>(m_renderer, texture, position);
    m_render_objects.push_back(object);
    return object;
}



std::shared_ptr<TextObject> ObjectRenderer::createText(const std::string& s, const Vector2 position, const float size, const float colour[3])
{
    auto object = std::make_shared<TextObject>(m_renderer, s, position, size, colour);
    m_render_objects.push_back(object);
    return object;
}



void ObjectRenderer::render()
{
    garbageCollect();

    for (auto& r : m_render_objects)
    {
        if (auto renderable = r.lock())
        {
            renderable->render();
        }
    }
}



void ObjectRenderer::garbageCollect()
{
    m_render_objects.erase(
        std::remove_if(
            m_render_objects.begin(),
            m_render_objects.end(),
            [](const std::weak_ptr<Renderable>& renderable) { return renderable.expired(); }
        ), m_render_objects.end());
}


