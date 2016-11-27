#include <iostream>
#include <algorithm>
#include <functional>

#include "StateGameplay.h"
#include "Game.h"
#include "ObjectBlock.h"
#include "AnimatedSprite.h"

StateGameplay::StateGameplay(ObjectFactory& _factory)
    : State(_factory)
    , player_lives(3)
    , player_speed(200)
    , player_projectile_speed(500)
    , player_shooting(false)
    , player_direction(MoveDirection::NONE)
    , alien_tick_delay(0)
    , alien_move_timer(0)
    , alien_shoot_delay(0)
    , alien_shoot_timer(0)
    , alien_side_speed(5)
    , alien_down_speed(20)
    , alien_projectile_speed(250)
    , aliens_direction(MoveDirection::RIGHT)
    , current_round(0)
    , reset_on_enter(true)
    , paused(false)
    , score(0)
    , score_multiplier(1)
    , mega_mode(false)
    , mega_mode_timer(0)
    , mega_mode_duration(3)
{
}



StateGameplay::~StateGameplay()
{
    deleteAllObjects();
}



void StateGameplay::onStateEnter()
{
    if (reset_on_enter)
    {
        reset_on_enter = false;
        resetState();
    }
    else if (paused)
    {
        paused = false;
        hideObjectsForPause(false);
    }
    else
    {
        deleteAllObjects();
    }
}



void StateGameplay::onStateLeave()
{
    if (reset_on_enter)
    {
        deleteAllObjects();
    }
    else if (paused)
    {
        hideObjectsForPause(true);
    }
    else
    {
        reset_on_enter = true;

        deleteAllObjects();
    }

    player_direction = MoveDirection::NONE;
}



void StateGameplay::tick(float _dt)
{
    collision_manager->tick();

    updateMegaMode(_dt);
    handleLifeBurn();

    handlePlayerMovement(_dt);
    handlePlayerShot();
    updatePlayerProjectile(_dt);

    handleAlienMovement(_dt);
    handleAlienShot(_dt);
    updateAlienProjectiles(_dt);

    updateScoreText();
    updateScoreMultText();
}



void StateGameplay::onCommand(const Command _command, const CommandState _command_state)
{
    if (_command == Command::MOVE_LEFT)
    {
        if (_command_state == CommandState::PRESSED || 
            _command_state == CommandState::REPEATING)
        {
            player_direction = MoveDirection::LEFT;
        }
        else
        {
            player_direction = MoveDirection::NONE;
        }
    }

    if (_command == Command::MOVE_RIGHT)
    {
        if (_command_state == CommandState::PRESSED || 
            _command_state == CommandState::REPEATING)
        {
            player_direction = MoveDirection::RIGHT;
        }
        else
        {
            player_direction = MoveDirection::NONE;
        }
    }

    if (_command == Command::SHOOT || _command == Command::MOVE_DOWN)
    {
        if (_command_state == CommandState::PRESSED || 
            _command_state == CommandState::REPEATING)
        {
            player_shooting = true;
        }
        else
        {
            player_shooting = false;
        }
    }

    if (_command == Command::MOVE_UP)
    {
        if (_command_state == CommandState::PRESSED)
        {
            life_burn = true;
        }
    }

    if (_command == Command::QUIT)
    {
        if (_command_state == CommandState::PRESSED)
        {
            getHandler()->pushState(GameState::START);
        }
    }

    if (_command == Command::PAUSE)
    {
        if (_command_state == CommandState::PRESSED)
        {
            paused = true;
            getHandler()->pushState(GameState::PAUSE);
        }
    }
}



bool StateGameplay::onCollision(SpriteObject* _object, SpriteObject* _other)
{
    if (_object->getCollisionType() == CollisionType::PROJECTILE &&
        _other->getCollisionType() == CollisionType::ALIEN)
    {
        if (!mega_mode)
        {
            player_projectile = nullptr;
        }

        aliens->removeObjectByPtr(_other);

        if (aliens->remainingObjects() == 0)
        {
            nextWave();
        }
        else
        {
            decreaseAlienTickDelay();
        }

        return true;
    }

    if (_object->getCollisionType() == CollisionType::ALIENPROJECTILE &&
        _other->getCollisionType() == CollisionType::SHIP)
    {
        garbageCollectAlienProjectiles(_object);

        removeLife();
        resetScoreMult();
        deactivateMegaMode();
        player->setPosition({ 100, player->getPosition().y });

        return true;
    }

    if (_object->getCollisionType() == CollisionType::PROJECTILE &&
        _other->getCollisionType() == CollisionType::BARRIER)
    {
        if (!mega_mode)
        {
            player_projectile = nullptr;
            resetScoreMult();
        }

        barrier_one->removeObjectByPtr(_other);
        barrier_two->removeObjectByPtr(_other);
        barrier_three->removeObjectByPtr(_other);

        return true;
    }

    if (_object->getCollisionType() == CollisionType::ALIENPROJECTILE &&
        _other->getCollisionType() == CollisionType::BARRIER)
    {
        garbageCollectAlienProjectiles(_object);

        barrier_one->removeObjectByPtr(_other);
        barrier_two->removeObjectByPtr(_other);
        barrier_three->removeObjectByPtr(_other);

        return true;
    }

    return false;
}



void StateGameplay::initCollisionManager()
{
    collision_manager = std::make_unique<CollisionManager>(std::bind(
        &StateGameplay::onCollision, this, std::placeholders::_1, std::placeholders::_2));

    getObjectFactory().linkCollisionManager(collision_manager.get());
}



void StateGameplay::initPlayer()
{
    Vector2 player_start{ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 };
    player = getObjectFactory().createSprite
        ("..\\..\\Resources\\Textures\\player.png", player_start, CollisionType::SHIP);
}



void StateGameplay::initHUD()
{
    score_title = getObjectFactory().createText("Score:", { 20, 30 }, 0.7f, 
        ASGE::COLOURS::DARKORANGE);

    score_text = getObjectFactory().createText(std::to_string(score), 
        score_title->getPosition(), 0.7f, ASGE::COLOURS::WHITE);
    score_text->modifyPosition({ 150, 0 });

    score_mult_title = getObjectFactory().createText("Multiplier:", { 330, 30 }, 0.7f, 
        ASGE::COLOURS::DARKORANGE);

    score_mult_text = getObjectFactory().createText(std::to_string(score_multiplier), 
        score_mult_title->getPosition(), 0.7f, ASGE::COLOURS::WHITE);
    score_mult_text->modifyPosition({ 260, 0 });

    mega_mode_bar = getObjectFactory().createText("", { 35, 700 }, 1.2f,
        ASGE::COLOURS::WHITE);

    lives_title = getObjectFactory().createText("Lives:", { 750, 30 }, 0.7f, 
        ASGE::COLOURS::DARKORANGE);

    Vector2 lives_pos{ WINDOW_WIDTH - 180, 5 };
    lives_block = std::make_unique<ObjectBlock>(lives_pos, 3, 4, 10, 20);

    std::string player_img = "..\\..\\Resources\\Textures\\player.png";
    for (int i = 0; i < player_lives; ++i)
    {
        lives_block->addObject(std::move
            (getObjectFactory().createSprite(player_img, { 0, 0 })));
    }
}



void StateGameplay::initAliens()
{
    Vector2 alien_start{ 100, 100.0f + (current_round * 10) };
    int max_rows = 1;
    int max_columns = 11;
    int padding_x = 10;
    int padding_y = 20;

    aliens = std::make_unique<ObjectBlock>(alien_start, max_rows, max_columns, 
        padding_x, padding_y);

    std::string alien_img = "..\\..\\Resources\\Textures\\top_alien_0.png";
    std::string alien_img2 = "..\\..\\Resources\\Textures\\top_alien_1.png";

    int score_value = 5;
    for (int row = 0; row < max_rows; ++row)
    {
        if (row == 1)
        {
            score_value = 2;

            alien_img = "..\\..\\Resources\\Textures\\middle_alien_0.png";
            alien_img2 = "..\\..\\Resources\\Textures\\middle_alien_1.png";
        }

        if (row == 3)
        {
            score_value = 1;

            alien_img = "..\\..\\Resources\\Textures\\bottom_alien_0.png";
            alien_img2 = "..\\..\\Resources\\Textures\\bottom_alien_1.png";
        }

        for (int col = 0; col < max_columns; ++col)
        {
            std::vector<std::unique_ptr<SpriteObject>> animationSprites;

            auto spr = getObjectFactory().createSprite(alien_img, alien_start, 
                CollisionType::ALIEN);

            auto spr2 = getObjectFactory().createSprite(alien_img2, alien_start, 
                CollisionType::ALIEN);

            spr->registerDeleteEvent([this, score_value]() 
                { score += score_value * score_multiplier; });

            spr->registerDeleteEvent([this]() { increaseScoreMult(); });

            animationSprites.emplace_back(std::move(spr));
            animationSprites.emplace_back(std::move(spr2));

            auto animatedSprite = std::make_unique<AnimatedSprite>(std::move
                (animationSprites));

            aliens->addObject(std::move(animatedSprite));
        }
    }

    aliens_direction = MoveDirection::RIGHT;
    generateAlienShootDelay();
}



void StateGameplay::initBarriers()
{
    std::string barrier_img = "..\\..\\Resources\\Textures\\barrier.png";

    int max_rows = 3;
    int max_columns = 12;
    int padding_x = 0;
    int padding_y = 0;

    makeBarrier(barrier_one, barrier_img, { 65, WINDOW_HEIGHT - 150 }, max_rows, 
        max_columns, padding_x, padding_y);

    makeBarrier(barrier_two, barrier_img, { 265, WINDOW_HEIGHT - 150 }, max_rows,
        max_columns, padding_x, padding_y);

    makeBarrier(barrier_three, barrier_img, { 465, WINDOW_HEIGHT - 150 }, max_rows,
        max_columns, padding_x, padding_y);
}



void StateGameplay::makeBarrier(std::unique_ptr<ObjectBlock>& _block,
    const std::string& _img, const Vector2 _pos, int _max_rows, int _max_columns,
    int _padding_x, int _padding_y) const
{
    _block = std::make_unique<ObjectBlock>(_pos, _max_rows, _max_columns, 
        _padding_x, _padding_y);

    for (int i = 0; i < _max_rows * _max_columns; ++i)
    {
        auto spr = getObjectFactory().createSprite(_img, { 0, 0 },
            CollisionType::BARRIER);

        _block->addObject(std::move(spr));
    }
}



void StateGameplay::handlePlayerShot()
{
    if (player_shooting && !player_projectile)
    {
        std::string projectile_img = "..\\..\\Resources\\Textures\\";
        projectile_img += mega_mode ? "projectile_mega.png" : "projectile.png";

        int mega_offset = mega_mode ? 2 : 0;
        Vector2 shoot_pos = { (player->getPosition().x + (player->getSize().x / 2)) 
            - mega_offset, player->getPosition().y - 5 };

        player_projectile = getObjectFactory().createSprite
            (projectile_img, shoot_pos, CollisionType::PROJECTILE);
    }
}



void StateGameplay::updatePlayerProjectile(float _dt)
{
    if (player_projectile)
    {
        player_projectile->modifyPosition({ 0, -player_projectile_speed * _dt });

        destroyPlayerProjectileAtScreenTop();
    }
}



void StateGameplay::destroyPlayerProjectileAtScreenTop()
{
    if (player_projectile && player_projectile->getPosition().y <= WINDOW_MARGIN)
    {
        player_projectile = nullptr;

        if (!mega_mode)
        {
            resetScoreMult();
        }
    }
}



void StateGameplay::handlePlayerMovement(float _dt) const
{
    if (player_direction == MoveDirection::LEFT)
    {
        if (player->getPosition().x > WINDOW_LEFT_BOUNDARY)
        {
            player->modifyPosition({ -player_speed * _dt, 0 });
        }
    }

    if (player_direction == MoveDirection::RIGHT)
    {
        if (player->getPosition().x + player->getSize().x < WINDOW_RIGHT_BOUNDARY)
        {
            player->modifyPosition({ player_speed * _dt, 0 });
        }
    }
}



void StateGameplay::handleAlienMovement(float _dt)
{
    alien_move_timer += _dt;

    if (alien_move_timer >= alien_tick_delay)
    {
        MoveDirection last_direction = aliens_direction;

        if (aliens->getEdgeLeft() <= WINDOW_LEFT_BOUNDARY ||
            aliens->getEdgeRight() >= WINDOW_RIGHT_BOUNDARY)
        {
            aliens_direction = MoveDirection::DOWN;
        }

        if (last_direction == MoveDirection::DOWN)
        {
            bool right_edge_closer = aliens->getEdgeLeft() - WINDOW_LEFT_BOUNDARY >
                -(aliens->getEdgeRight() - WINDOW_RIGHT_BOUNDARY);

            if (right_edge_closer)
            {
                aliens_direction = MoveDirection::LEFT;
            }
            else if (!right_edge_closer)
            {
                aliens_direction = MoveDirection::RIGHT;
            }
        }

        moveAliens(_dt);
        animateAliens();

        alien_move_timer = 0;
    }
}



void StateGameplay::moveAliens(float _dt)
{
    switch (aliens_direction)
    {
        case MoveDirection::DOWN:
        {
            aliens->moveBlock({ 0, alien_down_speed });
            determineInvasion();
            break;
        }

        case MoveDirection::LEFT:
        {
            aliens->moveBlock({ -alien_side_speed, 0 });
            break;
        }

        case MoveDirection::RIGHT:
        {
            aliens->moveBlock({ alien_side_speed, 0 });
            break;
        }

        default: {}
    }
}



void StateGameplay::animateAliens() const
{
    aliens->setNextAnimationFrame();
}



void StateGameplay::generateAlienShootDelay()
{
    float temp_delay = 0.25f / alien_tick_delay;
    float delay_modifier = temp_delay > 3.0f ? 3.0f : temp_delay;

    alien_shoot_delay = (static_cast<float>(rand()) /
        static_cast<float>(RAND_MAX / (4.0f - delay_modifier))) + 0.1f;
}



void StateGameplay::handleAlienShot(float _dt)
{
    alien_shoot_timer += _dt;

    if (alien_shoot_timer >= alien_shoot_delay)
    {
        Vector2 shoot_pos = aliens->getRandomShootingPosition();

        alien_projectiles.push_back(std::move(getObjectFactory().createSprite
            ("..\\..\\Resources\\Textures\\projectile.png", shoot_pos, 
            CollisionType::ALIENPROJECTILE)));
        
        generateAlienShootDelay();
        alien_shoot_timer = 0;
    }
}



void StateGameplay::updateAlienProjectiles(float _dt)
{
    for (auto& projectile : alien_projectiles)
    {
        projectile->modifyPosition({ 0, alien_projectile_speed * _dt });

        // Destroy projectile if it reaches the bottom of the screen.
        if (projectile && projectile->getPosition().y >= WINDOW_BOTTOM_BOUNDARY)
        {
            projectile = nullptr;
        }
    }

    garbageCollectAlienProjectiles(nullptr);
}



void StateGameplay::garbageCollectAlienProjectiles(SpriteObject* _object)
{
    alien_projectiles.erase(std::remove_if(
        alien_projectiles.begin(),
        alien_projectiles.end(),
        [_object](std::unique_ptr<SpriteObject>& spr) { return spr.get() == _object; }),
        alien_projectiles.end());
}



void StateGameplay::determineInvasion()
{
    if (aliens->getEdgeBottom() >= player->getPosition().y)
    {
        getHandler()->pushState(GameState::GAMEOVER);
    }
    else
    {
        decreaseAlienTickDelay();
    }
}



void StateGameplay::decreaseAlienTickDelay()
{
    if (alien_tick_delay >= 0.1f)
    {
        alien_tick_delay -= 0.007f;
    }
}



void StateGameplay::nextWave()
{
    ++current_round;

    alien_tick_delay += 0.3f;
    aliens_direction = MoveDirection::RIGHT;

    player_projectile = nullptr;
    alien_projectiles.clear();

    initAliens();
}



void StateGameplay::addLife()
{
    ++player_lives;

    if (lives_block)
    {
        lives_block->addObject(std::move(getObjectFactory().createSprite(
            "..\\..\\Resources\\Textures\\player.png", { 0, 0 })));
    }
}



void StateGameplay::removeLife()
{
    if (--player_lives <= 0)
    {
        reset_on_enter = true;

        getHandler()->pushState(GameState::GAMEOVER);
    }
    else
    {
        lives_block->popBack();
    }


}



void StateGameplay::resetState()
{
    current_round = 0;
    player_lives = 3;
    score = 0;
    alien_tick_delay = 0.5f;
    
    resetScoreMult();
    deactivateMegaMode();

    initCollisionManager();
    initPlayer();
    initHUD();
    initAliens();
    initBarriers();
}



void StateGameplay::deleteAllObjects()
{
    player = nullptr;
    player_projectile = nullptr;

    score_title = nullptr;
    score_text = nullptr;

    score_mult_title = nullptr;
    score_mult_text = nullptr;
    mega_mode_bar = nullptr;

    lives_title = nullptr;
    lives_block = nullptr;

    barrier_one = nullptr;
    barrier_two = nullptr;
    barrier_three = nullptr;

    if (aliens)
    {
        aliens->clear();
    }

    alien_projectiles.clear();

    // Collision manager needs to be destroyed last as it may have references to some
    // objects still active in the game.
    collision_manager = nullptr;
}



void StateGameplay::hideObjectsForPause(bool value) const
{
    bool actual = !value;

    if (player_projectile)
    {
        player_projectile->setVisible(actual);
    }

    aliens->setVisible(actual);
    for (auto& projectile : alien_projectiles)
    {
        projectile->setVisible(actual);
    }
}



void StateGameplay::updateScoreText() const
{
    score_text->setString(std::to_string(score));
}



void StateGameplay::updateScoreMultText() const
{
    score_mult_text->setString("x" + std::to_string(score_multiplier));
}



void StateGameplay::increaseScoreMult()
{
    if (++score_multiplier % 20 == 0)
    {
        activateMegaMode();
    }

    if (score_multiplier % 50 == 0)
    {
        addLife();
    }
}



void StateGameplay::resetScoreMult()
{
    score_multiplier = 1;
}



void StateGameplay::activateMegaMode()
{
    mega_mode = true;
    mega_mode_timer = mega_mode_duration;
    
    if (score_mult_text)
    {
        score_mult_text->setColour(ASGE::COLOURS::RED);
    }

    if (mega_mode_bar)
    {
        mega_mode_bar->setVisible(true);
    }
}



void StateGameplay::deactivateMegaMode()
{
    mega_mode = false;
    mega_mode_timer = 0;

    if (score_mult_text)
    {
        score_mult_text->setColour(ASGE::COLOURS::WHITE);
    }

    if (mega_mode_bar)
    {
        mega_mode_bar->setVisible(false);
    }
}



void StateGameplay::updateMegaMode(float _dt)
{
    if (mega_mode)
    {
        mega_mode_timer -= _dt;

        updateMegaModeBar();

        if (mega_mode_timer <= 0)
        {
            // Wait for the last mega mode projectile to expire
            if (player_projectile)
            {
                return;
            }

            deactivateMegaMode();
        }
    }
}



void StateGameplay::updateMegaModeBar() const
{
    mega_mode_bar->setString("");

    for (int i = 0; i < mega_mode_timer; ++i)
    {
        mega_mode_bar->appendString("|||||||||||||");
    }
}



void StateGameplay::handleLifeBurn()
{
    if (!life_burn)
    {
        return;
    }

    life_burn = false;

    if (player_projectile)
    {
        return;
    }

    if (player_lives > 1)
    {
        removeLife();
        activateMegaMode();
    }
}


