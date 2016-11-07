#pragma once
#include <Engine/Renderer.h>

#include "Vector2.h"

class Renderable
{
public:
    virtual ~Renderable() = default;

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
    Renderable(const std::shared_ptr<ASGE::Renderer>& _renderer)
        : renderer(_renderer)
        , visible(true)
    {
    }

    std::shared_ptr<ASGE::Renderer> renderer;
    bool visible;
};
