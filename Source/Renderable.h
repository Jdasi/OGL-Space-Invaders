#pragma once
#include <functional>

#include <Engine/Renderer.h>

#include "Vector2.h"

class Renderable
{
public:
    virtual ~Renderable();

    bool isVisible() const;
    void setVisible(bool _visible);

    Vector2 getPosition() const;
    void setPosition(const Vector2 _pos);
    void modifyPosition(const Vector2 _pos);

    virtual void render() = 0;

protected:
    Renderable(const std::shared_ptr<ASGE::Renderer>& _renderer, 
        std::function<void(Renderable*)> _delete_render_object, const Vector2 _position);

    std::shared_ptr<ASGE::Renderer> renderer;
    std::function<void(Renderable*)> delete_render_object;
    bool visible;
    Vector2 position;
};
