#pragma once
#include <memory>
#include <vector>
#include <atomic>

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

    void handlePlayerShot();
    void updatePlayerProjectile(float _dt);
    void destroyProjectileOnCollision(float _dt);
    void destroyProjectileAtScreenTop();
    void handlePlayerMovement(float _dt) const;

    void handleAlienMovement(float _dt);
    void moveAliens(float _dt);
    void animateAliens() const;
    void generateAlienShootDelay();
    void handleAlienShot(float _dt);
    void updateAlienProjectiles(float _dt);

    void decreaseAlienTickDelay(float _dt);
    void resetRound();
    void nextWave();
    void removeLife();

    void resetState();

    void deleteAllObjects();
    void hideObjectsForPause(bool value) const;
    void updatePlayerScore() const;

    std::unique_ptr<SpriteObject> player;
    std::unique_ptr<SpriteObject> player_projectile;

    std::unique_ptr<TextObject> score_title;
    std::unique_ptr<TextObject> score_text;
    std::unique_ptr<TextObject> lives_title;
    std::unique_ptr<ObjectBlock> lives_block;

    std::unique_ptr<ObjectBlock> aliens;
    std::vector<std::unique_ptr<SpriteObject>> alien_projectiles;

    int player_lives;
    float player_speed;
    float player_projectile_speed;
    std::atomic<bool> player_shooting;
    std::atomic<MoveDirection> player_direction;

    float alien_move_delay;
    float alien_move_timer;
    float alien_shoot_delay;
    float alien_shoot_timer;
    float alien_side_speed;
    float alien_down_speed;
    float alien_projectile_speed;
    MoveDirection aliens_direction;

    bool round_over;
    bool round_won;
    int current_round;
    bool reset_on_enter;
    std::atomic<bool> paused;
    int score;
};
