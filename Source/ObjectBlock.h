#include <memory>
#include <vector>

#include "Vector2.h"

class SpriteObject;

class ObjectBlock
{
public:
    ObjectBlock() = delete;
    ObjectBlock(Vector2 start_pos, int max_columns, int padding_x, int padding_y);
    ~ObjectBlock() = default;

    std::shared_ptr<SpriteObject> getObject(int id) const;
    void addObject(const std::shared_ptr<SpriteObject> object);
    void updateLayout();

    bool collisionTest(const std::shared_ptr<SpriteObject> other);
    void moveBlock(int x, int y);

    int getEdgeLeft() const;
    int getEdgeRight() const;
    int getEdgeBottom() const;

private:
    void updateEdges();

    std::vector<std::shared_ptr<SpriteObject>> m_objects;

    Vector2 m_start_pos;
    int m_max_columns;
    int m_padding_x;
    int m_padding_y;

    int m_edge_left;
    int m_edge_right;
    int m_edge_bottom;
};