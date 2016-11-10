#include <memory>
#include <vector>

#include "Vector2.h"

class SpriteObject;

class ObjectBlock
{
public:
    ObjectBlock() = delete;
    ObjectBlock(Vector2 _start_pos, int _max_columns, int _padding_x, int _padding_y,
        int _reserve_size = 0);
    ~ObjectBlock() = default;

    SpriteObject* getObject(unsigned int _id) const;
    Vector2 getRandomShootingPosition() const;
    void addObject(std::unique_ptr<SpriteObject> _object);
    void updateLayout();

    bool collisionTest(const SpriteObject& _other);
    void moveBlock(const Vector2 _pos);

    float getEdgeLeft() const;
    float getEdgeRight() const;
    float getEdgeBottom() const;

    int remainingObjects() const;

private:
    void updateEdges();
    void updateShootingPoints();

    Vector2 start_pos;
    int max_columns;
    int padding_x;
    int padding_y;

    float edge_left;
    float edge_right;
    float edge_bottom;

    std::vector<std::unique_ptr<SpriteObject>> objects;
    std::vector<Vector2> shooting_positions;
};
