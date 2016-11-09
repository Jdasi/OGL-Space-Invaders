#pragma once
#include <memory>

#include "State.h"

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
    explicit StateGameplay(ObjectFactory& _factory);
    virtual ~StateGameplay();
    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:
    void initPlayer();
    void initHUD();
    void initAliens();

    void playerShoot();
    void updatePlayerProjectile(float _dt);
    void movePlayer(float _dt) const;

    void updateAliensDirection(float _dt);
    void moveAliens(float _dt) const;

    void decreaseAlienTickDelay(float _dt);
    void resetRound();

    std::unique_ptr<SpriteObject> player;
    std::unique_ptr<SpriteObject> player_projectile;

    std::unique_ptr<TextObject> score_text;
    std::unique_ptr<ObjectBlock> aliens;

    int player_speed;
    int player_projectile_speed;
    bool player_shooting;
    MoveDirection player_direction;

    float alien_tick_delay;
    float alien_timer;
    int alien_side_speed;
    int alien_down_speed;
    int alien_projectile_speed;
    MoveDirection aliens_direction;
};
