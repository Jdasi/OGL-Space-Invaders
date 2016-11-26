#pragma once
#include <vector>

#include <Engine/Renderer.h>

#include "ObjectFactory.h"

class ObjectRenderer : public ObjectFactory
{
public:
    explicit ObjectRenderer(std::shared_ptr<ASGE::Renderer>& _renderer);
    virtual ~ObjectRenderer();

    std::unique_ptr<SpriteObject> createSprite
        (const std::string& _texture, const Vector2 _pos, 
        const CollisionType _collision_type) override;

    std::unique_ptr<TextObject> createText
        (const std::string& _str, const Vector2 _pos, const float _size, 
         const float _colour[3]) override;

    void deleteRenderObject(Renderable* _object);

    void render() const;

private:
    std::shared_ptr<ASGE::Renderer> renderer;
    std::vector<Renderable*> render_objects;
};
