#include "ObjectBlock.h"
#include "SpriteObject.h"
#include "Constants.h"

ObjectBlock::ObjectBlock(Vector2 start_pos, int max_columns, int padding_x, int padding_y)
    : m_start_pos(start_pos)
    , m_max_columns(max_columns - 1)
    , m_padding_x(padding_x)
    , m_padding_y(padding_y)
    , m_edge_left(0)
    , m_edge_right(0)
    , m_edge_bottom(0)
{
    if (m_max_columns < 0)
    {
        throw std::runtime_error("Error in ObjectBlock::ObjectBlock()");
    }
}



std::shared_ptr<SpriteObject> ObjectBlock::getObject(int id) const
{
    if (m_objects.empty() || id < 0 || id > m_objects.size())
    {
        throw std::runtime_error("Error in ObjectBlock::getObject()");
    }

    return m_objects[id];
}



void ObjectBlock::addObject(const std::shared_ptr<SpriteObject> object)
{
    m_objects.push_back(object);
}

void ObjectBlock::updateLayout()
{
    int column = 0;
    int row = 0;
    for (auto& obj : m_objects)
    {
        if (!obj)
        {
            continue;
        }

        obj->setPosition(m_start_pos);
        obj->modifyPosition((column * obj->getSize().x) + (column * m_padding_x), 
                            (row * obj->getSize().y) + (row * m_padding_y));

        if (column == m_max_columns)
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


bool ObjectBlock::collisionTest(std::shared_ptr<SpriteObject> other)
{
    for (auto& obj : m_objects)
    {
        if (!obj)
        {
            continue;
        }

        if (other->collisionTest(obj))
        {
            obj = nullptr;
            updateEdges();

            return true;
        }
    }

    return false;
}



void ObjectBlock::moveBlock(int x, int y)
{
    for (auto& obj : m_objects)
    {
        if (!obj)
        {
            continue;
        }

        obj->modifyPosition(x, y);
    }

    m_edge_left += x;
    m_edge_right += x;
    m_edge_bottom += y;
}



int ObjectBlock::getEdgeLeft() const
{
    return m_edge_left;
}



int ObjectBlock::getEdgeRight() const
{
    return m_edge_right;
}



int ObjectBlock::getEdgeBottom() const
{
    return m_edge_bottom;
}



void ObjectBlock::updateEdges()
{
    m_edge_left = WINDOW_WIDTH;
    m_edge_right = 0;
    m_edge_bottom = 0;

    for (auto& obj : m_objects)
    {
        if (!obj)
        {
            continue;
        }

        int obj_pos_left = obj->getPosition().x;
        if (obj_pos_left < m_edge_left)
        {
            m_edge_left = obj_pos_left;
        }

        int obj_pos_right = obj_pos_left + obj->getSize().x;
        if (obj_pos_right > m_edge_right)
        {
            m_edge_right = obj_pos_right;
        }

        int obj_pos_bottom = obj->getPosition().y + obj->getSize().y;
        if (obj_pos_bottom > m_edge_bottom)
        {
            m_edge_bottom = obj_pos_bottom;
        }
    }
}


