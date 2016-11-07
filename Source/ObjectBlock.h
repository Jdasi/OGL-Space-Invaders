#include <memory>
#include <vector>

#include "Vector2.h"

class SpriteObject;

class ObjectBlock
{
public:
    ObjectBlock() = delete;
    ObjectBlock(Vector2 _start_pos, int _max_columns, int _padding_x, int _padding_y, int _reserve_size = 0);
    ~ObjectBlock() = default;

    std::shared_ptr<SpriteObject> getObject(unsigned int _id) const;
    void addObject(const std::shared_ptr<SpriteObject>& _object);
    void updateLayout();

    bool collisionTest(const SpriteObject& _other);
    void moveBlock(int _x, int _y);

    int getEdgeLeft() const;
    int getEdgeRight() const;
    int getEdgeBottom() const;

private:
    void updateEdges();

    std::vector<std::shared_ptr<SpriteObject>> objects;

    Vector2 start_pos;
    int max_columns;
    int padding_x;
    int padding_y;

    int edge_left;
    int edge_right;
    int edge_bottom;
};