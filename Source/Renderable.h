#pragma once
#include <functional>

#include <Engine/Renderer.h>

#include "Vector2.h"

class Renderable
{
public:
    virtual ~Renderable()
    {
        delete_render_object(this);
    }

    bool isVisible() const
    {
        return visible;
    }

    void setVisible(bool _visible)
    {
        visible = _visible;
    }

    Vector2 getPosition() const
    {
        return position;
    }

    void setPosition(const Vector2 _pos)
    {
        position = _pos;
    }

    void modifyPosition(const Vector2 _pos)
    {
        position.x += _pos.x;
        position.y += _pos.y;
    }

    virtual void render() = 0;

protected:
    Renderable(const std::shared_ptr<ASGE::Renderer>& _renderer, 
        std::function<void(Renderable*)> _delete_render_object, const Vector2 _position)
        : renderer(_renderer)
        , delete_render_object(_delete_render_object)
        , visible(true)
        , position(_position)
    {
    }

    std::shared_ptr<ASGE::Renderer> renderer;
    std::function<void(Renderable*)> delete_render_object;
    bool visible;
    Vector2 position;
};
