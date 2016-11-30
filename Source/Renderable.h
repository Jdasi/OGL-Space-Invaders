#pragma once
#include <functional>

#include <Engine/Renderer.h>

#include "Vector2.h"

/* Base class for all SpriteObjects and TextObjects.
 * This is a convenience class to allow all sprites and text to be treated equally.
 * 
 * Only an ObjectFactory can create a Renderable.
 *
 * Each Renderable is responsible for rendering itself, hence the reference to the
 * game's renderer.
 *
 * Each Renderable is also responsible for deleting itself from the ObjectRenderer
 * once it falls out of scope, so it has a function pointer in order to do so.
 */
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
