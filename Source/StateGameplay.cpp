#include <iostream>

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
    , alien_tick_delay(1.0f)
    , alien_timer(0)
    , alien_side_speed(400)
    , alien_down_speed(800)
    , alien_projectile_speed(600)
    , aliens_direction(MoveDirection::RIGHT)
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
}



void StateGameplay::onStateLeave()
{
}



void StateGameplay::tick(float _dt)
{
    playerShoot();
    updatePlayerProjectile(_dt);
    movePlayer(_dt);
    updateAliensDirection(_dt);
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
    Vector2 player_start{ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 };
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



void StateGameplay::playerShoot()
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
        player_projectile->modifyPosition(0, -player_projectile_speed * _dt);

        // Destroy projectile if it collides with something.
        if (aliens->collisionTest(*player_projectile))
        {
            player_projectile = nullptr;

            if (aliens->remainingObjects() == 0)
            {
                resetRound();
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



void StateGameplay::movePlayer(float _dt) const
{
    if (player_direction == MoveDirection::LEFT)
    {
        if (player->getPosition().x > WINDOW_LEFT_BOUNDARY)
        {
            player->modifyPosition(-player_speed * _dt, 0);
        }
    }

    if (player_direction == MoveDirection::RIGHT)
    {
        if (player->getPosition().x + player->getSize().x < WINDOW_RIGHT_BOUNDARY)
        {
            player->modifyPosition(player_speed * _dt, 0);
        }
    }
}



void StateGameplay::updateAliensDirection(float _dt)
{
    if (alien_timer < alien_tick_delay)
    {
        alien_timer += _dt;
    }
    else
    {
        alien_timer = 0;

        // Move aliens down at left edge.
        bool left_edge_hit = aliens->getEdgeLeft() <= WINDOW_LEFT_BOUNDARY;
        if (left_edge_hit && aliens_direction != MoveDirection::DOWN)
        {
            aliens_direction = MoveDirection::DOWN;
        }
        else if (left_edge_hit)
        {
            aliens_direction = MoveDirection::RIGHT;
        }

        // Move aliens down at right edge.
        bool right_edge_hit = aliens->getEdgeRight() >= WINDOW_RIGHT_BOUNDARY;
        if (right_edge_hit && aliens_direction != MoveDirection::DOWN)
        {
            aliens_direction = MoveDirection::DOWN;
        }
        else if (right_edge_hit)
        {
            aliens_direction = MoveDirection::LEFT;
        }

        if (aliens_direction == MoveDirection::DOWN)
        {
            decreaseAlienTickDelay(_dt);
        }

        moveAliens(_dt);
    }
}



void StateGameplay::moveAliens(float _dt) const
{
    if (aliens_direction == MoveDirection::DOWN)
    {
        aliens->moveBlock(0, alien_down_speed * _dt);
    }

    if (aliens_direction == MoveDirection::LEFT)
    {
        aliens->moveBlock(-alien_side_speed * _dt, 0);
    }

    if (aliens_direction == MoveDirection::RIGHT)
    {
        aliens->moveBlock(alien_side_speed * _dt, 0);
    }
}



void StateGameplay::decreaseAlienTickDelay(float _dt)
{
    if (alien_tick_delay >= 0.1f)
    {
        alien_tick_delay -= _dt / 2;
    }
}



void StateGameplay::resetRound()
{
    initAliens();
    alien_tick_delay += 0.35f;
}


