#include "ObjectFactory.h"

ObjectFactory::ObjectFactory()
    : collision_manager(nullptr)
{
}



void ObjectFactory::linkCollisionManager(CollisionManager* _collision_manager)
{
    collision_manager = _collision_manager;
}


