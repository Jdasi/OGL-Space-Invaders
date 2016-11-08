#pragma once

class Renderable;

// Abstract class to avoid a circular reference with Renderable & Sprite/TextObjects.
class ObjectRendererDeleter
{
public:
    ObjectRendererDeleter() = default;
    virtual ~ObjectRendererDeleter() = default;

    virtual void DeleteRenderObject(Renderable* _object) = 0;

private:

};
