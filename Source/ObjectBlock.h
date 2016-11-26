#include <memory>
#include <vector>

#include "Vector2.h"

class AnimatedSprite;
class SpriteObject;

class ObjectBlock
{
public:
    ObjectBlock() = delete;
    ObjectBlock(Vector2 _start_pos, int _max_columns, int _max_rows, int _padding_x, int _padding_y);
    ~ObjectBlock() = default;

    AnimatedSprite* getObject(unsigned int _id) const;
    Vector2 getRandomShootingPosition() const;
    void addObject(std::unique_ptr<SpriteObject> _object);
    void addObject(std::unique_ptr<AnimatedSprite> _object);

    void moveBlock(const Vector2 _pos);

    float getEdgeLeft() const;
    float getEdgeRight() const;
    float getEdgeBottom() const;

    int remainingObjects() const;
    void popBack();

    void setVisible(bool b) const;
    void clear();

    void setNextAnimationFrame() const;

    void removeObjectByPtr(SpriteObject* object);

private:
    void updateLayout();
    void updateEdges();
    void updateShootingPoints();

    Vector2 start_pos;
    int max_columns;
    int padding_x;
    int padding_y;

    float edge_left;
    float edge_right;
    float edge_bottom;

    std::vector<std::unique_ptr<AnimatedSprite>> objects;
    std::vector<Vector2> shooting_positions;
};
