#include <algorithm>

#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(std::shared_ptr<ASGE::Renderer>& _renderer)
    : renderer(_renderer)
{
}



ObjectRenderer::~ObjectRenderer()
{
}



std::unique_ptr<SpriteObject> ObjectRenderer::createSprite(const std::string& _texture, 
    const Vector2 _pos)
{
    auto object = std::make_unique<SpriteObject>(renderer, *this, _texture, _pos);
    render_objects.push_back(object.get());
    return std::move(object);
}



std::unique_ptr<TextObject> ObjectRenderer::createText(const std::string& _str, 
    const Vector2 _pos, const float _size, const float _colour[3])
{
    auto object = std::make_unique<TextObject>(renderer, *this, _str, _pos, _size, _colour);
    render_objects.push_back(object.get());
    return std::move(object);
}



void ObjectRenderer::DeleteRenderObject(Renderable* object)
{
    for (auto iter = render_objects.begin(); iter != render_objects.end(); ++iter)
    {
        if (object == (*iter))
        {
            render_objects.erase(iter);
            break;
        }
    }
}



void ObjectRenderer::render()
{
    for (auto& r : render_objects)
    {
        if (r)        
        {
            r->render();
        }
    }
}
