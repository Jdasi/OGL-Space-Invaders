#pragma once
#include <memory>
#include <vector>
#include <atomic>

#include "State.h"
#include "CollisionManager.h"
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
    explicit StateGameplay(ObjectFactory& _factory);
    virtual ~StateGameplay();

    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;
    bool onCollision(SpriteObject* _object, SpriteObject* _other);

private:
    void initCollisionManager();
    void initPlayer();
    void initHUD();
    void initAliens();
    void initBarriers();
    void makeBarrier(std::unique_ptr<ObjectBlock>& _block, const std::string& _img, 
        const Vector2 _pos, int _max_rows, int _max_columns, int _padding_x, 
        int _padding_y) const;

    void handlePlayerShot();
    void updatePlayerProjectile(float _dt);
    void destroyPlayerProjectileAtScreenTop();
    void handlePlayerMovement(float _dt) const;

    void handleAlienMovement(float _dt);
    void moveAliens(float _dt);
    void animateAliens() const;
    void generateAlienShootDelay();
    void handleAlienShot(float _dt);
    void updateAlienProjectiles(float _dt);
    void garbageCollectAlienProjectiles(SpriteObject* _object);

    void determineInvasion();
    void decreaseAlienTickDelay();

    void nextWave();
    void removeLife();

    void resetState();

    void deleteAllObjects();
    void hideObjectsForPause(bool value) const;

    void updateScoreText() const;
    void updateScoreMultText() const;

    void increaseScoreMult();
    void resetScoreMult();
    void activateMegaMode();
    void deactivateMegaMode();
    void updateMegaMode(float _dt);

    std::unique_ptr<CollisionManager> collision_manager;

    std::unique_ptr<SpriteObject> player;
    std::unique_ptr<SpriteObject> player_projectile;

    std::unique_ptr<TextObject> score_title;
    std::unique_ptr<TextObject> score_text;

    std::unique_ptr<TextObject> score_mult_title;
    std::unique_ptr<TextObject> score_mult_text;
    std::unique_ptr<TextObject> mega_mode_bar;

    std::unique_ptr<TextObject> lives_title;
    std::unique_ptr<ObjectBlock> lives_block;

    std::unique_ptr<ObjectBlock> barrier_one;
    std::unique_ptr<ObjectBlock> barrier_two;
    std::unique_ptr<ObjectBlock> barrier_three;

    std::unique_ptr<ObjectBlock> aliens;
    std::vector<std::unique_ptr<SpriteObject>> alien_projectiles;

    int player_lives;
    float player_speed;
    float player_projectile_speed;
    std::atomic<bool> player_shooting;
    std::atomic<MoveDirection> player_direction;

    float alien_tick_delay;
    float alien_move_timer;
    float alien_shoot_delay;
    float alien_shoot_timer;
    float alien_side_speed;
    float alien_down_speed;
    float alien_projectile_speed;
    MoveDirection aliens_direction;

    int current_round;
    bool reset_on_enter;
    std::atomic<bool> paused;

    int score;
    int score_multiplier;
    bool mega_mode;
    float mega_mode_timer;
    float mega_mode_duration;
};
