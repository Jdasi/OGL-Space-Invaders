#pragma once
#include <memory>
#include <vector>
#include <atomic>

#include "State.h"
#include "CollisionManager.h"
#include "Vector2.h"
#include "RandomEngine.h"

class SpriteObject;
class TextObject;
class ObjectBlock;

/* Gameplay State where all of the game logic takes place. This is the bulk of the game.
 * This State owns the majority of SpriteObjects and TextObjects, which are created
 * through the use of the game's ObjectFactory.
 *
 * Only references to these objects are passed to the CollisionManager and ObjectFactory 
 * for the purposes of maintaining a list of collision objects and render objects.
 *
 * The Gameplay State is the only State that transitions to State Pause, so we handle 
 * that special functionality in this State's onStateEnter and onStateLeave events.
 */
class StateGameplay : public State
{
    enum class MoveDirection
    {
        LEFT,
        RIGHT,
        DOWN,
        NONE
    };

    enum class Edge
    {
        LEFT,
        RIGHT
    };

    enum class SoundEnabled
    {
        TRUE,
        FALSE
    };

public:
    explicit StateGameplay(GameData& _game_data);
    virtual ~StateGameplay();

    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;
    bool onCollision(SpriteObject* _object, SpriteObject* _other);

private:
    void initCollisionManager();
    void initTitles();
    void initPlayer();
    void initLives();

    void initAliens();
    void makeAlienBlock();

    void initBarriers();
    void makeBarrier(std::unique_ptr<ObjectBlock>& _block, const std::string& _img, 
        const Vector2 _pos, int _max_rows, int _max_columns, int _padding_x, 
        int _padding_y) const;

    void handlePlayerShot();
    void updatePlayerProjectile(float _dt);
    void destroyPlayerProjectileAtScreenTop();
    void handlePlayerMovement(float _dt) const;

    void handleAlienMovement(float _dt);
    void moveAliens(float _dt) const;
    void animateAliens() const;
    void generateAlienShootDelay();
    void handleAlienShot(float _dt);
    void updateAlienProjectiles(float _dt);
    void garbageCollectAlienProjectiles(SpriteObject* _object);

    void determineInvasion() const;
    void updateAlienTickDelay();
    void nextWave();

    void addLife(const SoundEnabled _setting = SoundEnabled::TRUE);
    void removeLife(const SoundEnabled _setting = SoundEnabled::TRUE);
    void respawnPlayer();

    void resetState();
    void destroyAllObjects();
    void hideObjectsForPause(bool _b) const;

    void updateScoreText() const;
    void updateScoreMultText() const;

    void increaseScore(int _amount);
    void increaseScoreMult();
    void resetScoreMult();

    void activateMegaMode();
    void deactivateMegaMode();
    void updateMegaMode(float _dt);
    void updateMegaModeBar() const;
    void handleLifeBurn();

    void updateExplosions(float _dt);
    void garbageCollectExplosions();
    void createExplosion(const Vector2 _pos);

    std::unique_ptr<CollisionManager> collision_manager;

    std::unique_ptr<SpriteObject> player;
    std::unique_ptr<SpriteObject> player_projectile;

    std::unique_ptr<TextObject> score_title;
    std::unique_ptr<TextObject> score_text;

    std::unique_ptr<TextObject> score_multiplier_title;
    std::unique_ptr<TextObject> score_multiplier_text;
    std::unique_ptr<TextObject> mega_mode_bar;

    std::unique_ptr<TextObject> lives_title;
    std::unique_ptr<ObjectBlock> lives_block;
    std::unique_ptr<TextObject> lives_text;

    std::unique_ptr<ObjectBlock> barrier_one;
    std::unique_ptr<ObjectBlock> barrier_two;
    std::unique_ptr<ObjectBlock> barrier_three;

    std::unique_ptr<ObjectBlock> aliens;
    std::vector<std::unique_ptr<SpriteObject>> alien_projectiles;
    std::vector<std::unique_ptr<SpriteObject>> explosions;

    unsigned int player_lives;
    float player_speed;
    float player_projectile_speed;
    std::atomic<bool> player_shooting;
    std::atomic<MoveDirection> player_direction;
    std::atomic<bool> life_burn;

    float alien_tick_delay;
    float alien_move_timer;
    float alien_shoot_delay;
    float alien_shoot_timer;
    float alien_side_speed;
    float alien_down_speed;
    float alien_projectile_speed;
    MoveDirection aliens_direction;
    Edge aliens_last_edge_hit;

    unsigned int current_wave;
    bool reset_on_enter;
    std::atomic<bool> paused;
    bool apply_score;

    unsigned int score_since_last_life;
    unsigned int score_multiplier;
    bool mega_mode;
    float mega_mode_timer;
    float mega_mode_duration;

    RandomEngine random_engine;
};
