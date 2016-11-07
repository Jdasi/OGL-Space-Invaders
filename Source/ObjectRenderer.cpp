#include <algorithm>

#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(std::shared_ptr<ASGE::Renderer>& _renderer)
    : renderer(_renderer)
{
}



std::shared_ptr<SpriteObject> ObjectRenderer::createSprite(const std::string& _texture, const Vector2 _pos)
{
    auto object = std::make_shared<SpriteObject>(renderer, _texture, _pos);
    render_objects.push_back(object);
    return object;
}



std::shared_ptr<TextObject> ObjectRenderer::createText(const std::string& _str, const Vector2 _pos, const float _size, const float _colour[3])
{
    auto object = std::make_shared<TextObject>(renderer, _str, _pos, _size, _colour);
    render_objects.push_back(object);
    return object;
}



void ObjectRenderer::render()
{
    garbageCollect();

    for (auto& r : render_objects)
    {
        if (auto renderable = r.lock())
        {
            renderable->render();
        }
    }
}



void ObjectRenderer::garbageCollect()
{
    render_objects.erase(
        std::remove_if(
            render_objects.begin(),
            render_objects.end(),
            [](const std::weak_ptr<Renderable>& renderable) { return renderable.expired(); }
        ), render_objects.end());
}


