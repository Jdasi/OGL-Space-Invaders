#include <iostream>
#include <algorithm>

#include "StateGameplay.h"
#include "Game.h"
#include "Constants.h"
#include "ObjectBlock.h"

StateGameplay::StateGameplay(ObjectFactory& _factory)
    : State(_factory)
    , player(nullptr)
    , player_projectile(nullptr)
    , score_text(nullptr)
    , aliens(nullptr)
    , player_speed(400)
    , player_projectile_speed(500)
    , player_shooting(false)
    , player_direction(MoveDirection::NONE)
    , alien_move_delay(1)
    , alien_move_timer(0)
    , alien_shoot_delay(0)
    , alien_shoot_timer(0)
    , alien_side_speed(5)
    , alien_down_speed(20)
    , alien_projectile_speed(500)
    , aliens_direction(MoveDirection::RIGHT)
    , round_over(false)
    , round_won(false)
{
}



StateGameplay::~StateGameplay()
{
}



void StateGameplay::onStateEnter()
{
    initPlayer();
    initHUD();
    initAliens();

    generateAlienShootDelay();
}



void StateGameplay::onStateLeave()
{
}



void StateGameplay::tick(float _dt)
{
    handlePlayerMovement(_dt);
    handlePlayerShot();
    updatePlayerProjectile(_dt);

    handleAlienMovement(_dt);
    handleAlienShot(_dt);
    updateAlienProjectiles(_dt);

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
}



void StateGameplay::initPlayer()
{
    Vector2 player_start{ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50 };
    player = getObjectFactory().createSprite
        ("..\\..\\Resources\\Textures\\player.png", player_start);
}



void StateGameplay::initHUD()
{
    score_text = getObjectFactory().createText
        ("Score:", { 20, 30 }, 0.7f, ASGE::COLOURS::DARKORANGE);
}



void StateGameplay::initAliens()
{
    Vector2 alien_start{ 100, 100 };
    int max_rows = 5;
    int max_columns = 11;
    int padding_x = 10;
    int padding_y = 20;

    aliens = std::make_unique<ObjectBlock>
        (alien_start, max_columns, padding_x, padding_y, max_rows * max_columns);

    std::string alien_img = "..\\..\\Resources\\Textures\\top_alien_0.png";
    for (int row = 0; row < max_rows; ++row)
    {
        if (row == 1)
        {
            alien_img = "..\\..\\Resources\\Textures\\middle_alien_0.png";
        }

        if (row == 3)
        {
            alien_img = "..\\..\\Resources\\Textures\\bottom_alien_0.png";
        }

        for (int col = 0; col < max_columns; ++col)
        {
            aliens->addObject
                (std::move(getObjectFactory().createSprite(alien_img, alien_start)));
        }
    }

    aliens->updateLayout();
}



void StateGameplay::handlePlayerShot()
{
    if (player_shooting && !player_projectile)
    {
        player_projectile = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\projectile.png", { player->getPosition().x + (player->getSize().x / 2), player->getPosition().y - 5 });
    }
}



void StateGameplay::updatePlayerProjectile(float _dt)
{
    if (player_projectile)
    {
        player_projectile->modifyPosition({ 0, -player_projectile_speed * _dt });

        // Destroy projectile if it collides with something.
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

        // Destroy projectile if it reaches the top of the screen.
        if (player_projectile && player_projectile->getPosition().y <= WINDOW_MARGIN)
        {
            player_projectile = nullptr;
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
    if (alien_move_timer < alien_move_delay)
    {
        alien_move_timer += _dt;
    }
    else
    {
        MoveDirection aliens_prev_direction = aliens_direction;

        bool left_edge_hit = aliens->getEdgeLeft() <= WINDOW_LEFT_BOUNDARY;
        bool right_edge_hit = aliens->getEdgeRight() >= WINDOW_RIGHT_BOUNDARY;
        
        if (left_edge_hit || right_edge_hit)
        {
            aliens_direction = MoveDirection::DOWN;
        }

        if (aliens_prev_direction == MoveDirection::DOWN)
        {
            if (left_edge_hit)
            {
                aliens_direction = MoveDirection::RIGHT;
            }
            else if (right_edge_hit)
            {
                aliens_direction = MoveDirection::LEFT;
            }
        }

        moveAliens(_dt);
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



void StateGameplay::generateAlienShootDelay()
{
    alien_shoot_delay = 0.1f + static_cast<float>(rand()) /
        static_cast<float>(RAND_MAX / 4);
}



void StateGameplay::handleAlienShot(float _dt)
{
    if (alien_shoot_timer < alien_shoot_delay)
    {
        alien_shoot_timer += _dt;
    }
    else
    {
        // Find the position of an alien that can shoot.
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
        alien_move_delay -= _dt / 2;
    }
}



void StateGameplay::resetRound()
{
    if (round_won)
    {
        initAliens();
        alien_move_delay += 0.35f;
        aliens_direction = MoveDirection::RIGHT;
    }
    else
    {
        initPlayer();
    }

    round_over = false;
    round_won = false;
}


