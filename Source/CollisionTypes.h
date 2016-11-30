#pragma once

/* Used by SpriteObjects and the CollisionManager to identify which SpriteObjects
 * need to be included in the game's collision detection.
 */
enum class CollisionType
{
    PROJECTILE,
    ALIENPROJECTILE,
    SHIP,
    ALIEN,
    BARRIER,
    NONE
};
