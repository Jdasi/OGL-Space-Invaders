#pragma once
#include <vector>

#include <Engine/Renderer.h>

#include "ObjectFactory.h"

/* Maintains a list of all active Renderables.
 * InvadersGame calls ObjectRenderers render functio each cycle, which then calls render 
 * on every Renderable in the list so that they're actually drawn to the screen.
 *
 * Each Renderable is responsible for deleting itself from the list of render objects
 * once it falls out of scope.
 */
class ObjectRenderer : public ObjectFactory
{
public:
    explicit ObjectRenderer(std::shared_ptr<ASGE::Renderer>& _renderer);
    virtual ~ObjectRenderer();

    std::unique_ptr<SpriteObject> createSprite
        (const std::string& _texture, const Vector2 _pos, 
        const CollisionType _collision_type, float _lifetime) override;

    std::unique_ptr<TextObject> createText
        (const std::string& _str, const Vector2 _pos, const float _size, 
         const float _colour[3]) override;

    void deleteRenderObject(Renderable* _object);

    void render() const;

private:
    std::shared_ptr<ASGE::Renderer> renderer;
    std::vector<Renderable*> render_objects;
};
