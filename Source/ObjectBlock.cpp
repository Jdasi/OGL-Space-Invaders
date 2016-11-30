#include <algorithm>

#include "ObjectBlock.h"
#include "AnimatedSprite.h"
#include "Constants.h"

ObjectBlock::ObjectBlock(const Vector2 _start_pos, int _max_rows, int _max_columns, 
    int _padding_x, int _padding_y)
    : start_pos(_start_pos)
    , max_columns(_max_columns)
    , padding_x(_padding_x)
    , padding_y(_padding_y)
    , edge_left(0)
    , edge_right(0)
    , edge_bottom(0)
{
    objects.reserve(_max_columns * _max_rows);
    shooting_positions.reserve(_max_columns);
}



AnimatedSprite* ObjectBlock::getObject(unsigned int _id) const
{
    if (objects.empty() || _id > objects.size())
    {
        return nullptr;
    }

    return objects[_id].get();
}



Vector2 ObjectBlock::getRandomShootingPosition() const
{
    return shooting_positions[rand() % shooting_positions.size()];
}



/* Convenience function to allow for SpriteObjects to be added in the same way
 * that AnimatedSprites are.
 * In an ideal world, ObjectBlock would be templated to avoid the unecessary
 * littering of vectors.
 */
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



/* Moves all of the objects owned by the block, and also updates interal
 * variables that keep track of the shooting positions and edges.
 * The last two things are especially important for the aliens.
 */
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



void ObjectBlock::setVisible(bool _b) const
{
    for (auto& obj : objects)
    {
        obj->setVisible(_b);
    }
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
        [object](const std::unique_ptr<AnimatedSprite>& anim_spr) { return anim_spr->ownsSpriteObject(object); }),
        objects.end());

    updateEdges();
    updateShootingPoints();
}



void ObjectBlock::popBack()
{
    objects.pop_back();

    updateEdges();
    updateShootingPoints();
}



void ObjectBlock::clear()
{
    objects.clear();
}



/* Formats the block of objects based on the passed dimensions and padding values.
 * Objects will be neatly laid out based on these variables, depending on how many
 * exist in the vector.
 */
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



/* Emulates a sort of BoundingBox for the entirety of the ObjectBlock.
 * We're not concerned with the top edge in this version of Space Invaders, though.
 */
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

    findAllRows();
    findLowestPointInAllColumns();
}



void ObjectBlock::findAllRows()
{
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
}



void ObjectBlock::findLowestPointInAllColumns()
{
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


