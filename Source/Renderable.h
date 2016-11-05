#pragma once
#include <Engine/Renderer.h>

#include "Vector2.h"

class Renderable
{
public:
    virtual ~Renderable() = default;

    bool isVisible() const
    {
        return m_visible;
    }

    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    virtual Vector2 getPosition() const = 0;
    virtual void setPosition(const Vector2 position) = 0;
    virtual void modifyPosition(int x, int y) = 0;

    virtual void render() = 0;

protected:
    Renderable(const std::shared_ptr<ASGE::Renderer>& renderer)
        : m_renderer(renderer)
        , m_visible(true)
    {
    }

    std::shared_ptr<ASGE::Renderer> m_renderer;
    bool m_visible;
};
