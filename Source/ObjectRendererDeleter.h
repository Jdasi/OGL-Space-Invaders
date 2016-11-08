#pragma once

class Renderable;

class ObjectRendererDeleter
{
public:
    ObjectRendererDeleter() = default;
    virtual ~ObjectRendererDeleter() = default;

    virtual void DeleteRenderObject(Renderable* object) = 0;
};
