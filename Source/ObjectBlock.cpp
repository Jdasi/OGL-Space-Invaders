#include <algorithm>

#include "ObjectBlock.h"
#include "AnimatedSprite.h"
#include "Constants.h"

ObjectBlock::ObjectBlock(Vector2 _start_pos, int _max_rows, int _max_columns, 
    int _padding_x, int _padding_y)
    : start_pos(_start_pos)
    , max_columns(_max_columns)
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

    objects.reserve(_max_columns * _max_rows);
    shooting_positions.reserve(_max_columns);
}



AnimatedSprite* ObjectBlock::getObject(unsigned int _id) const
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
    std::vector<std::unique_ptr<SpriteObject>> animationSprites;
    animationSprites.emplace_back(std::move(_object));

    auto animatedSprite = std::make_unique<AnimatedSprite>(std::move(animationSprites));
    addObject(std::move(animatedSprite));
}



void ObjectBlock::addObject(std::unique_ptr<AnimatedSprite> _object)
{
    objects.emplace_back(std::move(_object));

    updateLayout();
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



void ObjectBlock::popBack()
{
    objects.pop_back();
}



void ObjectBlock::setVisible(bool b) const
{
    for (auto& obj : objects)
    {
        obj->setVisible(b);
    }
}



void ObjectBlock::clear()
{
    objects.clear();
}



void ObjectBlock::setNextAnimationFrame() const
{
    for (auto& obj : objects)
    {
        obj->setNextAnimationFrame();
    }
}



void ObjectBlock::removeObjectByPtr(SpriteObject* object)
{
    objects.erase(std::remove_if(
        objects.begin(),
        objects.end(),
        [object](const std::unique_ptr<AnimatedSprite>& anim_spr) { return anim_spr->containsSpriteObject(object); }),
        objects.end());

    updateEdges();
    updateShootingPoints();
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

        if (++column >= max_columns)
        {
            column = 0;
            ++row;
        }
    }

    updateEdges();
    updateShootingPoints();
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


