#pragma once
#include <vector>

#include <Engine/Renderer.h>

#include "ObjectFactory.h"

class ObjectRenderer : public ObjectFactory
{
public:
    explicit ObjectRenderer(const std::shared_ptr<ASGE::Renderer>& renderer);
    virtual ~ObjectRenderer() = default;

    std::shared_ptr<SpriteObject> createSprite
        (const std::string& texture, const Vector2 position) override;

    std::shared_ptr<TextObject> createText
        (const std::string& s, const Vector2 position, const float size, 
         const float colour[3]) override;

    void render();

private:
    void garbageCollect();

    std::shared_ptr<ASGE::Renderer> m_renderer;
    std::vector<std::weak_ptr<Renderable>> m_renderObjects;
};
