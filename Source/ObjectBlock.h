#include <memory>
#include <vector>

#include "Vector2.h"

class AnimatedSprite;
class SpriteObject;

/* A container for a group of AnimatedSprites.
 * For convenience, SpriteObjects are converted to AnimatedSprites before being added. 
 * Though this is not ideal. Time constraints, man.
 * 
 * This allows for a whole "block" of sprites to be handled as one.
 * Additional information is kept to identify the block's most important edges,
 * as well as firing positions if the block is able to shoot.
 */
class ObjectBlock
{
public:
    ObjectBlock() = delete;
    ObjectBlock(const Vector2 _start_pos, int _max_columns, int _max_rows, int _padding_x, 
        int _padding_y);
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

    void setVisible(bool _b) const;
    void setNextAnimationFrame() const;

    void removeObjectByPtr(SpriteObject* object);
    void popBack();
    void clear();

private:
    void updateLayout();
    void updateEdges();

    void updateShootingPoints();
    void findAllRows();
    void findLowestPointInAllColumns();

    Vector2 start_pos;
    unsigned int max_columns;
    int padding_x;
    int padding_y;

    float edge_left;
    float edge_right;
    float edge_bottom;

    std::vector<std::unique_ptr<AnimatedSprite>> objects;
    std::vector<Vector2> shooting_positions;
};
