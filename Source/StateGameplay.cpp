#include <iostream>
#include <algorithm>

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
    , alien_move_delay(0.9f)
    , alien_move_timer(0)
    , alien_shoot_delay(0)
    , alien_shoot_timer(0)
    , alien_side_speed(5)
    , alien_down_speed(20)
    , alien_projectile_speed(250)
    , aliens_direction(MoveDirection::RIGHT)
    , round_over(false)
    , round_won(false)
    , current_round(0)
    , reset_on_enter(true)
    , paused(false)
    , score(0)
{
}



StateGameplay::~StateGameplay()
{
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


void StateGameplay::resetState()
{
    player_lives = 3;
    score = 0;

    initPlayer();
    initHUD();
    initAliens();
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
    handlePlayerMovement(_dt);
    handlePlayerShot();
    updatePlayerProjectile(_dt);

    handleAlienMovement(_dt);
    handleAlienShot(_dt);
    updateAlienProjectiles(_dt);

    updatePlayerScore();

    if (round_over)
    {
        resetRound();
    }
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

    if (_command == Command::SHOOT)
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



void StateGameplay::initPlayer()
{
    Vector2 player_start{ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 };
    player = getObjectFactory().createSprite
        ("..\\..\\Resources\\Textures\\player.png", player_start);
}



void StateGameplay::initHUD()
{
    score_title = getObjectFactory().createText
        ("Score:", { 20, 30 }, 0.7f, ASGE::COLOURS::DARKORANGE);

    score_text = getObjectFactory().createText
        (std::to_string(score), score_title->getPosition(), 0.7f, ASGE::COLOURS::WHITE);
    score_text->modifyPosition({ 150, 0 });

    lives_title = getObjectFactory().createText
        ("Lives:", { 750, 30 }, 0.7f, ASGE::COLOURS::DARKORANGE);

    Vector2 lives_pos{ WINDOW_WIDTH - 180, 5 };
    lives_block = std::make_unique<ObjectBlock>(lives_pos, 4, 10, 20, 4 * 2);

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
    int max_rows = 5;
    int max_columns = 11;
    int padding_x = 10;
    int padding_y = 20;

    aliens = std::make_unique<ObjectBlock>
        (alien_start, max_columns, padding_x, padding_y, max_rows * max_columns);

    std::string alien_img = "..\\..\\Resources\\Textures\\top_alien_0.png";
    std::string alien_img2 = "..\\..\\Resources\\Textures\\top_alien_1.png";

    int scorevalue = 40;
    for (int row = 0; row < max_rows; ++row)
    {
        if (row == 1)
        {
            scorevalue = 20;

            alien_img = "..\\..\\Resources\\Textures\\middle_alien_0.png";
            alien_img2 = "..\\..\\Resources\\Textures\\middle_alien_1.png";
        }

        if (row == 3)
        {
            scorevalue = 10;

            alien_img = "..\\..\\Resources\\Textures\\bottom_alien_0.png";
            alien_img2 = "..\\..\\Resources\\Textures\\bottom_alien_1.png";
        }

        for (int col = 0; col < max_columns; ++col)
        {
            std::vector<std::unique_ptr<SpriteObject>> animationSprites;
            animationSprites.emplace_back(
                std::move(getObjectFactory().createSprite(alien_img, alien_start, 
                [this, scorevalue](){ score += scorevalue; })));

            animationSprites.emplace_back(
                std::move(getObjectFactory().createSprite(alien_img2, alien_start)));

            auto animatedSprite = std::make_unique<AnimatedSprite>(std::move
                (animationSprites));

            aliens->addObject(std::move(animatedSprite));
        }
    }

    aliens_direction = MoveDirection::RIGHT;
    generateAlienShootDelay();
}



void StateGameplay::handlePlayerShot()
{
    if (player_shooting && !player_projectile)
    {
        player_projectile = getObjectFactory().createSprite
            ("..\\..\\Resources\\Textures\\projectile.png", 
            { player->getPosition().x + (player->getSize().x / 2), 
            player->getPosition().y - 5 });
    }
}



void StateGameplay::updatePlayerProjectile(float _dt)
{
    if (player_projectile)
    {
        player_projectile->modifyPosition({ 0, -player_projectile_speed * _dt });

        destroyProjectileOnCollision(_dt);
        destroyProjectileAtScreenTop();
    }
}



void StateGameplay::destroyProjectileOnCollision(float _dt)
{
    if (aliens->collisionTest(*player_projectile))
    {
        player_projectile = nullptr;

        if (aliens->remainingObjects() == 0)
        {
            round_over = true;
            round_won = true;
        }
        else
        {
            decreaseAlienTickDelay(_dt);
        }
    }
}



void StateGameplay::destroyProjectileAtScreenTop()
{
    if (player_projectile && player_projectile->getPosition().y <= WINDOW_MARGIN)
    {
        player_projectile = nullptr;
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

    if (alien_move_timer >= alien_move_delay)
    {
        MoveDirection aliens_prev_direction = aliens_direction;
        
        if (aliens->getEdgeLeft() <= WINDOW_LEFT_BOUNDARY || 
            aliens->getEdgeRight() >= WINDOW_RIGHT_BOUNDARY)
        {
            aliens_direction = MoveDirection::DOWN;
        }

        bool right_edge_closer = aliens->getEdgeLeft() - WINDOW_LEFT_BOUNDARY > 
            -(aliens->getEdgeRight() - WINDOW_RIGHT_BOUNDARY);

        if (aliens_prev_direction == MoveDirection::DOWN)
        {
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
            decreaseAlienTickDelay(_dt);
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
    float temp_delay = 0.25f / alien_move_delay;
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
            ("..\\..\\Resources\\Textures\\projectile.png", shoot_pos)));
        
        generateAlienShootDelay();
        alien_shoot_timer = 0;
    }
}



void StateGameplay::updateAlienProjectiles(float _dt)
{
    for (auto& projectile : alien_projectiles)
    {
        projectile->modifyPosition({ 0, alien_projectile_speed * _dt });

        if (projectile->collisionTest(*player))
        {
            projectile = nullptr;
            round_over = true;
            round_won = false;
        }

        // Destroy projectile if it reaches the bottom of the screen.
        if (projectile && projectile->getPosition().y >= WINDOW_BOTTOM_BOUNDARY)
        {
            projectile = nullptr;
        }
    }

    // Clean up any projectiles that don't point to data.
    alien_projectiles.erase(std::remove_if(
        alien_projectiles.begin(), 
        alien_projectiles.end(), 
        [](std::unique_ptr<SpriteObject>& spr) { return spr == nullptr; } ), 
        alien_projectiles.end());
}



void StateGameplay::decreaseAlienTickDelay(float _dt)
{
    if (alien_move_delay >= 0.1f)
    {
        alien_move_delay -= 0.007f;
    }
}



void StateGameplay::resetRound()
{
    round_over = false;

    if (round_won)
    {
        round_won = false;

        nextWave();
    }
    else
    {
        removeLife();
        initPlayer();
    }
}



void StateGameplay::nextWave()
{
    ++player_lives;
    ++current_round;

    alien_move_delay += 0.3f;
    aliens_direction = MoveDirection::RIGHT;

    std::string player_img = "..\\..\\Resources\\Textures\\player.png";
    lives_block->addObject(std::move
        (getObjectFactory().createSprite(player_img, { 0, 0 })));

    alien_projectiles.clear();

    initAliens();
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



void StateGameplay::deleteAllObjects()
{
    player = nullptr;
    player_projectile = nullptr;

    score_title = nullptr;
    score_text = nullptr;
    lives_title = nullptr;
    lives_block = nullptr;

    aliens->clear();
    alien_projectiles.clear();
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



void StateGameplay::updatePlayerScore() const
{
    score_text->setString(std::to_string(score));
}


