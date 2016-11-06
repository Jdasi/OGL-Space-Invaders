#pragma once
#include <memory>
#include <vector>

#include "State.h"
#include "Vector2.h"

class SpriteObject;
class TextObject;
class ObjectBlock;

enum class MoveDirection
{
    LEFT,
    RIGHT,
    DOWN,
    NONE
};

class StateGameplay : public State
{
public:
    explicit StateGameplay(ObjectFactory& factory);
    virtual ~StateGameplay();
    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float dt) override;
    void onCommand(const Command c, const CommandState s) override;

private:
    void initPlayer();
    void initHUD();
    void initAliens();

    void playerShoot();
    void updatePlayerProjectile(float dt);
    void movePlayer(float dt) const;

    void updateAliensDirection(float dt);
    void moveAliens(float dt) const;

    std::shared_ptr<SpriteObject> m_player;
    std::shared_ptr<SpriteObject> m_player_projectile;

    std::shared_ptr<TextObject> m_score_text;
    std::unique_ptr<ObjectBlock> m_aliens;

    int m_player_speed;
    int m_player_projectile_speed;
    bool m_player_shooting;
    MoveDirection m_player_direction;

    float m_alien_tick_delay;
    float m_alien_timer;
    int m_alien_side_speed;
    int m_alien_down_speed;
    int m_alien_projectile_speed;
    MoveDirection m_aliens_direction;
};
