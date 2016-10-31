#include <algorithm>

#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(const std::shared_ptr<ASGE::Renderer>& renderer)
    : m_renderer(renderer)
{
}



std::shared_ptr<SpriteObject> ObjectRenderer::createSprite(const std::string& texture, const Position position)
{
    auto object = std::make_shared<SpriteObject>(m_renderer, texture, position);
    m_renderObjects.push_back(object);
    return object;
}



std::shared_ptr<TextObject> ObjectRenderer::createText(const std::string& s, const Position position, const float size, const float colour[3])
{
    auto object = std::make_shared<TextObject>(m_renderer, s, position, size, colour);
    m_renderObjects.push_back(object);
    return object;
}



void ObjectRenderer::render()
{
    garbageCollect();

    for (auto& r : m_renderObjects)
    {
        if (auto renderable = r.lock())
        {
            renderable->render();
        }
    }
}



void ObjectRenderer::garbageCollect()
{
    m_renderObjects.erase(
        std::remove_if(
            m_renderObjects.begin(),
            m_renderObjects.end(),
            [](const std::weak_ptr<Renderable>& renderable) { return renderable.expired(); }
        ), m_renderObjects.end());
}


