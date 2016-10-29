#pragma once
#include <memory>

#include <Engine/Sprite.h>

class CollisionObject
{

public:

private:
    CollisionObject();
    ~CollisionObject();

    std::unique_ptr<ASGE::Sprite> sprite;
};