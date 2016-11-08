#pragma once
#include <vector>

#include <Engine/Renderer.h>

#include "ObjectFactory.h"
#include "ObjectRendererDeleter.h"

class ObjectRenderer : public ObjectFactory, public ObjectRendererDeleter
{
public:
    explicit ObjectRenderer(std::shared_ptr<ASGE::Renderer>& _renderer);
    virtual ~ObjectRenderer();

    std::unique_ptr<SpriteObject> createSprite
        (const std::string& _texture, const Vector2 _pos) override;

    std::unique_ptr<TextObject> createText
        (const std::string& _str, const Vector2 _pos, const float _size, 
         const float _colour[3]) override;

    void DeleteRenderObject(Renderable* object) override;

    void render();

private:
    std::shared_ptr<ASGE::Renderer> renderer;
    std::vector<Renderable*> render_objects;
};
