#pragma once
#include <array>
#include <memory>

#include <Engine/Renderer.h>

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual std::array<int, 2> getPosition() = 0;
    virtual void setPosition(std::array<int, 2> pos) = 0;

    virtual float getScale() = 0;
    virtual void setScale(float f) = 0;

    virtual std::shared_ptr<ASGE::Renderer> getRenderer() = 0;
    virtual void setRenderer(std::shared_ptr<ASGE::Renderer> r) = 0;

    virtual void render() = 0;

private:
    std::shared_ptr<ASGE::Renderer> m_renderer;
};