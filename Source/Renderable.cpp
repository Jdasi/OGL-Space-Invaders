#include "Renderable.h"

Renderable::Renderable(const std::shared_ptr<ASGE::Renderer>& _renderer,
    std::function<void(Renderable*)> _delete_render_object, const Vector2 _position)
    : renderer(_renderer)
    , delete_render_object(_delete_render_object)
    , visible(true)
    , position(_position)
{
}



Renderable::~Renderable()
{
    delete_render_object(this);
}



bool Renderable::isVisible() const
{
    return visible;
}



void Renderable::setVisible(bool _visible)
{
    visible = _visible;
}



Vector2 Renderable::getPosition() const
{
    return position;
}



void Renderable::setPosition(const Vector2 _pos)
{
    position = _pos;
}



void Renderable::modifyPosition(const Vector2 _pos)
{
    position.x += _pos.x;
    position.y += _pos.y;
}


