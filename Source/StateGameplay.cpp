#include <iostream>
#include <algorithm>
#include <functional>

#include "StateGameplay.h"
#include "Game.h"
#include "ObjectBlock.h"
#include "AnimatedSprite.h"
#include "JMath.h"

StateGameplay::StateGameplay(GameData& _game_data)
    : State(_game_data)
    , player_lives(0)
    , player_speed(PLAYER_SPEED)
    , player_projectile_speed(PLAYER_PROJECTILE_SPEED)
    , player_shooting(false)
    , player_direction(MoveDirection::NONE)
    , alien_tick_delay(0)
    , alien_move_timer(0)
    , alien_shoot_delay(0)
    , alien_shoot_timer(0)
    , alien_side_speed(ALIEN_SIDE_SPEED)
    , alien_down_speed(ALIEN_DOWN_SPEED)
    , alien_projectile_speed(ALIEN_PROJECTILE_SPEED)
    , aliens_direction(MoveDirection::RIGHT)
    , aliens_last_edge_hit(Edge::LEFT)
    , current_wave(0)
    , reset_on_enter(true)
    , paused(false)
    , apply_score(true)
    , score_since_last_life(0)
    , score_multiplier(0)
    , mega_mode(false)
    , mega_mode_timer(0)
    , mega_mode_duration(MEGA_MODE_DURATION)
{
}



StateGameplay::~StateGameplay()
{
    destroyAllObjects();
}



void StateGameplay::onStateEnter()
{
    apply_score = true;

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
        destroyAllObjects();
    }
}



void StateGameplay::onStateLeave()
{
    apply_score = false;

    if (paused)
    {
        hideObjectsForPause(true);
    }
    else
    {
        reset_on_enter = true;
        destroyAllObjects();
    }

    player_direction = MoveDirection::NONE;
}



void StateGameplay::tick(float _dt)
{
    collision_manager->tick();

    updateExplosions(_dt);

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

        createExplosion(_other->getPosition());
        aliens->removeObjectByPtr(_other);

        if (aliens->remainingObjects() == 0)
        {
            nextWave();
        }
        else
        {
            updateAlienTickDelay();
        }

        return true;
    }

    if (_object->getCollisionType() == CollisionType::ALIENPROJECTILE &&
        _other->getCollisionType() == CollisionType::SHIP)
    {
        garbageCollectAlienProjectiles(_object);

        respawnPlayer();

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

    gameData().object_factory->linkCollisionManager(collision_manager.get());
}



void StateGameplay::initTitles()
{
    // Score TextObjects.
    score_title = gameData().object_factory->createText("Score:", { 20, 30 }, 0.7f,
        ASGE::COLOURS::DARKORANGE);

    score_text = gameData().object_factory->createText(std::to_string(gameData().score),
        score_title->getPosition(), 0.7f, ASGE::COLOURS::WHITE);
    score_text->modifyPosition({ 150, 0 });

    // Score Multiplier TextObjects.
    score_multiplier_title = gameData().object_factory->createText("Multiplier:", 
        { 330, 30 }, 0.7f, ASGE::COLOURS::DARKORANGE);

    score_multiplier_text = gameData().object_factory->createText(
        std::to_string(score_multiplier), score_multiplier_title->getPosition(), 
        0.7f, ASGE::COLOURS::WHITE);
    score_multiplier_text->modifyPosition({ 260, 0 });

    // Other TextObjects.
    lives_title = gameData().object_factory->createText("Lives:", { 750, 30 }, 0.7f,
        ASGE::COLOURS::DARKORANGE);

    lives_text = gameData().object_factory->createText("0", lives_title->getPosition(), 
        1.5f, ASGE::COLOURS::WHITE);
    lives_text->modifyPosition({ 175, 20 });

    mega_mode_bar = gameData().object_factory->createText("", { 35, 700 }, 1.15f,
        ASGE::COLOURS::WHITE);
}



void StateGameplay::initPlayer()
{
    Vector2 player_start{ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 };
    player = gameData().object_factory->createSprite
        (TEXTURE_PATH + PLAYER_IMG, player_start, CollisionType::SHIP);
}



void StateGameplay::initLives()
{
    player_lives = 0;

    Vector2 lives_pos{ WINDOW_WIDTH - 180, 5 };
    int max_rows = 3;
    int max_columns = 4;
    int padding_x = 10;
    int padding_y = 10;

    lives_block = std::make_unique<ObjectBlock>(lives_pos, max_rows, max_columns,
        padding_x, padding_y);

    for (int i = 0; i < PLAYER_START_LIVES; ++i)
    {
        addLife(SoundEnabled::FALSE);
    }
}



void StateGameplay::initAliens()
{
    aliens_direction = MoveDirection::RIGHT;
    aliens_last_edge_hit = Edge::LEFT;

    makeAlienBlock();
    updateAlienTickDelay();
    generateAlienShootDelay();
}



void StateGameplay::makeAlienBlock()
{
    Vector2 alien_start{ 100, 100.0f + (current_wave * 10) };
    int max_rows = ALIEN_ROWS_MAX;
    int max_columns = ALIEN_COLUMNS_MAX;
    int padding_x = 10;
    int padding_y = 20;

    aliens = std::make_unique<ObjectBlock>(alien_start, max_rows, max_columns,
        padding_x, padding_y);

    std::string alien_frame_0 = TEXTURE_PATH + TOP_ALIEN_IMG_0;
    std::string alien_frame_1 = TEXTURE_PATH + TOP_ALIEN_IMG_1;

    int value = TOP_ALIEN_VALUE;
    for (int row = 0; row < max_rows; ++row)
    {
        if (row == 1)
        {
            value = MIDDLE_ALIEN_VALUE;

            alien_frame_0 = TEXTURE_PATH + MIDDLE_ALIEN_IMG_0;
            alien_frame_1 = TEXTURE_PATH + MIDDLE_ALIEN_IMG_1;
        }

        if (row == 3)
        {
            value = BOTTOM_ALIEN_VALUE;

            alien_frame_0 = TEXTURE_PATH + BOTTOM_ALIEN_IMG_0;
            alien_frame_1 = TEXTURE_PATH + BOTTOM_ALIEN_IMG_1;
        }

        for (int col = 0; col < max_columns; ++col)
        {
            // Setup sprites.
            auto spr_0 = gameData().object_factory->createSprite(alien_frame_0,
                alien_start, CollisionType::ALIEN);

            auto spr_1 = gameData().object_factory->createSprite(alien_frame_1,
                alien_start, CollisionType::ALIEN);

            // Setup delete events.
            spr_0->registerDeleteEvent([this, value]() { increaseScore(value); });

            spr_0->registerDeleteEvent([this]() { increaseScoreMult(); });

            // Setup vector.
            std::vector<std::unique_ptr<SpriteObject>> animationSprites;

            animationSprites.emplace_back(std::move(spr_0));
            animationSprites.emplace_back(std::move(spr_1));

            auto animatedSprite = std::make_unique<AnimatedSprite>(std::move(
                animationSprites));

            // Add to ObjectBlock.
            aliens->addObject(std::move(animatedSprite));
        }
    }
}



void StateGameplay::initBarriers()
{
    std::string barrier_img = TEXTURE_PATH + BARRIER_IMG;

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
        auto spr = gameData().object_factory->createSprite(_img, { 0, 0 },
            CollisionType::BARRIER);

        _block->addObject(std::move(spr));
    }
}



void StateGameplay::handlePlayerShot()
{
    if (player_shooting && !player_projectile)
    {
        std::string projectile_img = TEXTURE_PATH;
        projectile_img += mega_mode ? PROJECTILE_MEGA_IMG : PROJECTILE_IMG;

        std::string projectile_sound = AUDIO_PATH;
        projectile_sound += mega_mode ? PLAYER_SHOT_MEGA_CUE : PLAYER_SHOT_CUE;
        gameData().audio_engine->play2D(projectile_sound.c_str(), false);

        int mega_offset = mega_mode ? 2 : 0;
        Vector2 shoot_pos = { (player->getPosition().x + (player->getSize().x / 2)) 
            - mega_offset, player->getPosition().y - 5 };

        player_projectile = gameData().object_factory->createSprite
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

        if (aliens->getEdgeLeft() <= WINDOW_LEFT_BOUNDARY)
        {
            aliens_direction = MoveDirection::DOWN;
            aliens_last_edge_hit = Edge::LEFT;
        }
        else if (aliens->getEdgeRight() >= WINDOW_RIGHT_BOUNDARY)
        {
            aliens_direction = MoveDirection::DOWN;
            aliens_last_edge_hit = Edge::RIGHT;
        }

        if (last_direction == MoveDirection::DOWN)
        {
            if (aliens_last_edge_hit == Edge::RIGHT)
            {
                aliens_direction = MoveDirection::LEFT;
            }
            else if (aliens_last_edge_hit == Edge::LEFT)
            {
                aliens_direction = MoveDirection::RIGHT;
            }
        }

        moveAliens(_dt);
        animateAliens();

        alien_move_timer = 0;
    }
}



void StateGameplay::moveAliens(float _dt) const
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
    float delay_modifier = (aliens->remainingObjects() - 1) * ALIEN_SHOOT_DELAY_MIN;

    alien_shoot_delay = random_engine.randomFloat(ALIEN_SHOOT_DELAY_MIN, 
        ALIEN_SHOOT_DELAY_SOFT_MAX + delay_modifier);
}



void StateGameplay::handleAlienShot(float _dt)
{
    alien_shoot_timer += _dt;

    if (alien_shoot_timer >= alien_shoot_delay)
    {
        gameData().audio_engine->play2D((AUDIO_PATH + ALIEN_SHOT_CUE).c_str(), false);

        Vector2 shoot_pos = aliens->getRandomShootingPosition();

        alien_projectiles.push_back(std::move(gameData().object_factory->createSprite
            (TEXTURE_PATH + PROJECTILE_IMG, shoot_pos, 
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



void StateGameplay::determineInvasion() const
{
    if (aliens->getEdgeBottom() >= player->getPosition().y)
    {
        getHandler()->pushState(GameState::GAMEOVER);
    }
}



void StateGameplay::updateAlienTickDelay()
{
    float temp_delay = aliens->remainingObjects() * ALIEN_TICK_DELAY_MIN;
    float tick_delay_max = ALIEN_TICK_DELAY_ROOF / (current_wave * CURRENT_WAVE_FACTOR);

    alien_tick_delay = JMath::clamp(temp_delay, ALIEN_TICK_DELAY_MIN, tick_delay_max);
}



void StateGameplay::nextWave()
{
    ++current_wave;

    aliens_direction = MoveDirection::RIGHT;

    player_projectile = nullptr;
    alien_projectiles.clear();
    
    initAliens();
}



void StateGameplay::addLife(const SoundEnabled _setting)
{
    ++player_lives;

    if (_setting == SoundEnabled::TRUE)
    {
        gameData().audio_engine->play2D((AUDIO_PATH + EXTRA_LIFE_CUE).c_str(), false);
    }

    if (!lives_block)
    {
        return;
    }

    if (lives_block->remainingObjects() < MAX_DISPLAYED_LIVES)
    {
        lives_block->setVisible(true);
        lives_text->setVisible(false);

        lives_block->addObject(std::move(gameData().object_factory->createSprite(
            TEXTURE_PATH + PLAYER_IMG, { 0, 0 })));
    }
    else
    {
        lives_block->setVisible(false);
        lives_text->setVisible(true);
    }

    lives_text->setString(std::to_string(player_lives));
}



void StateGameplay::removeLife(const SoundEnabled _setting)
{
    --player_lives;

    if (_setting == SoundEnabled::TRUE)
    {
        gameData().audio_engine->play2D((AUDIO_PATH + PLAYER_DEATH_CUE).c_str(), false);
    }

    if (player_lives <= 0)
    {
        reset_on_enter = true;

        getHandler()->pushState(GameState::GAMEOVER);
    }
    else if (player_lives < MAX_DISPLAYED_LIVES)
    {
        lives_block->popBack();
    }
    else if (player_lives <= MAX_DISPLAYED_LIVES)
    {
        lives_block->setVisible(true);
        lives_text->setVisible(false);
    }

    lives_text->setString(std::to_string(player_lives));
}



void StateGameplay::respawnPlayer()
{
    removeLife();
    resetScoreMult();
    deactivateMegaMode();
    player->setPosition({ 100, player->getPosition().y });
}



void StateGameplay::resetState()
{
    current_wave = 0;

    score_since_last_life = 0;
    gameData().score = 0;
    gameData().highest_score_multiplier = 0;
    
    resetScoreMult();
    deactivateMegaMode();

    initCollisionManager();
    initTitles();
    initPlayer();
    initLives();
    initAliens();
    initBarriers();
}



void StateGameplay::destroyAllObjects()
{
    player = nullptr;
    player_projectile = nullptr;

    score_title = nullptr;
    score_text = nullptr;

    score_multiplier_title = nullptr;
    score_multiplier_text = nullptr;
    mega_mode_bar = nullptr;

    lives_title = nullptr;
    lives_block = nullptr;
    lives_text = nullptr;

    barrier_one = nullptr;
    barrier_two = nullptr;
    barrier_three = nullptr;

    if (aliens)
    {
        aliens->clear();
        aliens = nullptr;
    }

    alien_projectiles.clear();
    explosions.clear();

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

    for (auto& explosion : explosions)
    {
        explosion->setVisible(actual);
    }
}



void StateGameplay::updateScoreText() const
{
    score_text->setString(std::to_string(gameData().score));
}



void StateGameplay::updateScoreMultText() const
{
    score_multiplier_text->setString("x" + std::to_string(score_multiplier));
}



void StateGameplay::increaseScore(int _amount)
{
    if (!apply_score)
    {
        return;
    }

    int multiplied_score = _amount * score_multiplier;

    score_since_last_life += multiplied_score;
    gameData().score += multiplied_score;

    if (score_since_last_life >= EXTRA_LIFE_THRESHOLD)
    {
        addLife();
        score_since_last_life = 0 + (gameData().score % EXTRA_LIFE_THRESHOLD);
    }
}



void StateGameplay::increaseScoreMult()
{
    if (!apply_score)
    {
        return;
    }

    ++score_multiplier;

    if (score_multiplier % MEGA_MODE_THRESHOLD == 0)
    {
        activateMegaMode();
    }

    if (score_multiplier > gameData().highest_score_multiplier)
    {
        gameData().highest_score_multiplier = score_multiplier;
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

    gameData().audio_engine->play2D((AUDIO_PATH + POWER_UP_CUE).c_str(), false);
    
    if (score_multiplier_text)
    {
        score_multiplier_text->setColour(ASGE::COLOURS::RED);
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

    gameData().audio_engine->play2D((AUDIO_PATH + POWER_DOWN_CUE).c_str(), false);

    if (score_multiplier_text)
    {
        score_multiplier_text->setColour(ASGE::COLOURS::WHITE);
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
    std::string temp_str;
    mega_mode_bar->setString("");

    // 0.15f gives a smooth enough transition.
    for (float i = 0; i < mega_mode_timer; i += 0.15f)
    {
        temp_str.append("||");
    }

    mega_mode_bar->setString(temp_str);
}



void StateGameplay::handleLifeBurn()
{
    if (!life_burn)
    {
        return;
    }

    life_burn = false;

    // Stop mega projectiles behaviour conflicting with normal projectiles.
    if (player_projectile && !mega_mode)
    {
        return;
    }

    if (player_lives > 1)
    {
        removeLife();
        activateMegaMode();
    }
}



void StateGameplay::updateExplosions(float _dt)
{
    for (auto& explosion : explosions)
    {
        explosion->tick(_dt);
    }

    garbageCollectExplosions();
}



void StateGameplay::garbageCollectExplosions()
{
    explosions.erase(std::remove_if(
        explosions.begin(),
        explosions.end(),
        [](std::unique_ptr<SpriteObject>& spr) { return spr->expired(); }),
        explosions.end());
}



void StateGameplay::createExplosion(Vector2 _pos)
{
    gameData().audio_engine->play2D((AUDIO_PATH + ALIEN_DEATH_CUE).c_str(), false);

    auto spr = gameData().object_factory->createSprite(
        TEXTURE_PATH + EXPLOSION_IMG, _pos, CollisionType::NONE, 0.25f);
    spr->setScale(1.1f);

    explosions.push_back(std::move(spr));
}


