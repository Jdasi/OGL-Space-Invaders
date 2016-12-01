#include <algorithm>
#include <functional>

#include "ObjectRenderer.h"
#include "CollisionTypes.h"
#include "CollisionManager.h"

ObjectRenderer::ObjectRenderer(std::shared_ptr<ASGE::Renderer>& _renderer)
    : renderer(_renderer)
{
}



ObjectRenderer::~ObjectRenderer()
{
}



std::unique_ptr<SpriteObject> ObjectRenderer::createSprite(const std::string& _texture, 
    const Vector2 _pos, const CollisionType _collision_type, float _lifetime)
{
    auto object = std::make_unique<SpriteObject>(renderer, 
        std::bind(&ObjectRenderer::deleteRenderObject, this, std::placeholders::_1),
         _texture, _pos, _collision_type, _lifetime);

    if (collision_manager)
    {
        // The CollisionManager only wants objects that need collision.
        if (object->getCollisionType() != CollisionType::NONE)
        {
            collision_manager->addCollisionObject(object.get());
        }
    }

    render_objects.push_back(object.get());
    return std::move(object);
}



std::unique_ptr<TextObject> ObjectRenderer::createText(const std::string& _str, 
    const Vector2 _pos, const float _size, const float _colour[3])
{
    auto object = std::make_unique<TextObject>(renderer, 
        std::bind(&ObjectRenderer::deleteRenderObject, this, std::placeholders::_1), 
        _str, _pos, _size, _colour);
    render_objects.push_back(object.get());

    return std::move(object);
}



/* Function given to all Renderables so that they can remove themselves from the 
 * render objects vector when they fall out of scope.
 */
void ObjectRenderer::deleteRenderObject(Renderable* object)
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



void ObjectRenderer::render() const
{
    for (auto& r : render_objects)
    {
        r->render();
    }
}
