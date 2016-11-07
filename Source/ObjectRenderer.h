#pragma once
#include <vector>

#include <Engine/Renderer.h>

#include "ObjectFactory.h"

class ObjectRenderer : public ObjectFactory
{
public:
    explicit ObjectRenderer(std::shared_ptr<ASGE::Renderer>& _renderer);
    virtual ~ObjectRenderer() = default;

    std::shared_ptr<SpriteObject> createSprite
        (const std::string& _texture, const Vector2 _pos) override;

    std::shared_ptr<TextObject> createText
        (const std::string& _str, const Vector2 _pos, const float _size, 
         const float _colour[3]) override;

    void render();

private:
    void garbageCollect();

    std::shared_ptr<ASGE::Renderer> renderer;
    std::vector<std::weak_ptr<Renderable>> render_objects;
};
