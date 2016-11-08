#pragma once
#include <Engine/Renderer.h>

#include "Vector2.h"
#include "ObjectRendererDeleter.h"

class Renderable
{
public:
    virtual ~Renderable()
    {
        object_renderer_deleter.DeleteRenderObject(this);
    }

    bool isVisible() const
    {
        return visible;
    }

    void setVisible(bool _visible)
    {
        visible = _visible;
    }

    virtual Vector2 getPosition() const = 0;
    virtual void setPosition(const Vector2 _pos) = 0;
    virtual void modifyPosition(int _x, int _y) = 0;

    virtual void render() = 0;

protected:
    Renderable(const std::shared_ptr<ASGE::Renderer>& _renderer, ObjectRendererDeleter& _object_renderer_deleter)
        : renderer(_renderer)
        , object_renderer_deleter(_object_renderer_deleter)
        , visible(true)
    {
    }

    std::shared_ptr<ASGE::Renderer> renderer;
    ObjectRendererDeleter& object_renderer_deleter;
    bool visible;
};
