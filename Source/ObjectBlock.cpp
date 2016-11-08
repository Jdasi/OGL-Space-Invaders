#include <algorithm>

#include "ObjectBlock.h"
#include "SpriteObject.h"
#include "Constants.h"

ObjectBlock::ObjectBlock(Vector2 _start_pos, int _max_columns, int _padding_x, int _padding_y, int _reserve_size)
    : start_pos(_start_pos)
    , max_columns(_max_columns - 1)
    , padding_x(_padding_x)
    , padding_y(_padding_y)
    , edge_left(0)
    , edge_right(0)
    , edge_bottom(0)
{
    if (max_columns < 0)
    {
        throw std::runtime_error("Error in ObjectBlock::ObjectBlock()");
    }

    objects.reserve(_reserve_size);
}



SpriteObject* ObjectBlock::getObject(unsigned int _id) const
{
    if (objects.empty() || _id > objects.size())
    {
        throw std::runtime_error("Error in ObjectBlock::getObject()");
    }

    return objects[_id].get();
}



void ObjectBlock::addObject(std::unique_ptr<SpriteObject> _object)
{
    objects.emplace_back(std::move(_object));
}



void ObjectBlock::updateLayout()
{
    int column = 0;
    int row = 0;
    for (auto& obj : objects)
    {
        obj->setPosition(start_pos);
        obj->modifyPosition((column * obj->getSize().x) + (column * padding_x), 
                            (row * obj->getSize().y) + (row * padding_y));

        if (column == max_columns)
        {
            column = 0;
            ++row;
        }
        else
        {
            ++column;
        }
    }

    updateEdges();
}



bool ObjectBlock::collisionTest(const SpriteObject& _other)
{
    for (auto iter = objects.begin(); iter != objects.end(); ++iter)
    {
        if ((*iter)->collisionTest(_other))
        {
            objects.erase(iter);
            updateEdges();

            return true;
        }
    }

    return false;
}



void ObjectBlock::moveBlock(int _x, int _y)
{
    for (auto& obj : objects)
    {
        obj->modifyPosition(_x, _y);
    }

    edge_left += _x;
    edge_right += _x;
    edge_bottom += _y;
}



int ObjectBlock::getEdgeLeft() const
{
    return edge_left;
}



int ObjectBlock::getEdgeRight() const
{
    return edge_right;
}



int ObjectBlock::getEdgeBottom() const
{
    return edge_bottom;
}



void ObjectBlock::updateEdges()
{
    edge_left = WINDOW_WIDTH;
    edge_right = 0;
    edge_bottom = 0;

    for (auto& obj : objects)
    {
        int obj_pos_left = obj->getPosition().x;
        if (obj_pos_left < edge_left)
        {
            edge_left = obj_pos_left;
        }

        int obj_pos_right = obj_pos_left + obj->getSize().x;
        if (obj_pos_right > edge_right)
        {
            edge_right = obj_pos_right;
        }

        int obj_pos_bottom = obj->getPosition().y + obj->getSize().y;
        if (obj_pos_bottom > edge_bottom)
        {
            edge_bottom = obj_pos_bottom;
        }
    }
}


