#pragma once
#include <Engine/Renderer.h>

#include "Position.h"

class Renderable
{
public:
    virtual ~Renderable() = default;

    void setIsVisible(bool visible)
    {
        m_visible = visible;
    }

    bool getIsVisible() const
    {
        return m_visible;
    }

    virtual void setPosition(const Position position) = 0;
    virtual Position getPosition() const = 0;

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
