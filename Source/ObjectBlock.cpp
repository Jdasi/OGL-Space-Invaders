#include <algorithm>

#include "ObjectBlock.h"
#include "SpriteObject.h"
#include "Constants.h"

ObjectBlock::ObjectBlock(Vector2 _start_pos, int _max_columns, int _padding_x, 
    int _padding_y, int _reserve_size)
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

    objects.reserve(_max_columns);
    shooting_positions.reserve(_max_columns);
}



SpriteObject* ObjectBlock::getObject(unsigned int _id) const
{
    if (objects.empty() || _id > objects.size())
    {
        throw std::runtime_error("Error in ObjectBlock::getObject()");
    }

    return objects[_id].get();
}



Vector2 ObjectBlock::getRandomShootingPosition() const
{
    return shooting_positions[rand() % shooting_positions.size()];
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
        obj->modifyPosition({ (column * obj->getSize().x) + (column * padding_x), 
                            (row * obj->getSize().y) + (row * padding_y) });

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
    updateShootingPoints();
}



bool ObjectBlock::collisionTest(const SpriteObject& _other)
{
    for (auto iter = objects.begin(); iter != objects.end(); ++iter)
    {
        if ((*iter)->collisionTest(_other))
        {
            objects.erase(iter);
            updateEdges();
            updateShootingPoints();

            return true;
        }
    }

    return false;
}



void ObjectBlock::moveBlock(const Vector2 _pos)
{
    for (auto& obj : objects)
    {
        obj->modifyPosition(_pos);
    }

    for (auto& position : shooting_positions)
    {
        position.x += _pos.x;
        position.y += _pos.y;
    }

    edge_left += _pos.x;
    edge_right += _pos.x;
    edge_bottom += _pos.y;
}



float ObjectBlock::getEdgeLeft() const
{
    return edge_left;
}



float ObjectBlock::getEdgeRight() const
{
    return edge_right;
}



float ObjectBlock::getEdgeBottom() const
{
    return edge_bottom;
}



int ObjectBlock::remainingObjects() const
{
    return objects.size();
}



void ObjectBlock::updateEdges()
{
    edge_left = WINDOW_WIDTH;
    edge_right = 0;
    edge_bottom = 0;

    for (auto& obj : objects)
    {
        float obj_pos_left = obj->getPosition().x;
        if (obj_pos_left < edge_left)
        {
            edge_left = obj_pos_left;
        }

        float obj_pos_right = obj_pos_left + obj->getSize().x;
        if (obj_pos_right > edge_right)
        {
            edge_right = obj_pos_right;
        }

        float obj_pos_bottom = obj->getPosition().y + obj->getSize().y;
        if (obj_pos_bottom > edge_bottom)
        {
            edge_bottom = obj_pos_bottom;
        }
    }
}



void ObjectBlock::updateShootingPoints()
{
    shooting_positions.clear();

    // Find all the rows.
    for (auto& obj : objects)
    {
        Vector2 obj_pos = { obj->getPosition().x + (obj->getSize().x / 2), 0 };

        if (std::find(shooting_positions.begin(), shooting_positions.end(), obj_pos) 
            == shooting_positions.end())
        {
            shooting_positions.push_back(obj_pos);
        }
        else
        {
            // We have all the rows.
            break;
        }
    }

    // Find the lowest position in each column, offset y position by object size.
    for (auto& obj : objects)
    {
        Vector2 obj_size = obj->getSize();
        Vector2 obj_pos = obj->getPosition();

        for (auto& shooting_pos : shooting_positions)
        {
            if (obj_pos.x + (obj_size.x / 2) == shooting_pos.x && 
                obj_pos.y > shooting_pos.y)
            {
                shooting_pos.y = obj_pos.y + obj_size.y;
            }
        }
    }
}


