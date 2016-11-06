#include <iostream>

#include "StateGameplay.h"
#include "Game.h"
#include "Constants.h"

StateGameplay::StateGameplay(ObjectFactory& factory)
    : State(factory)
    , m_player_start({ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 })
    , m_player_speed(400)
    , m_player_projectile_speed(600)
    , m_player_shooting(false)
    , m_player_direction(MoveDirection::NONE)
{
}



StateGameplay::~StateGameplay()
{
}



void StateGameplay::onStateEnter()
{
    m_player = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\player.png", m_player_start);
    m_text = getObjectFactory().createText("Score:", { 20, 30 }, 0.7f, ASGE::COLOURS::DARKORANGE);

    int max_rows = 5;
    int max_columns = 11;
    int paddingX = 10;
    int paddingY = 20;
    Vector2 alien_start{ 100, 100 };

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
            m_aliens.push_back(getObjectFactory().createSprite(alien_img, alien_start));

            auto spr = m_aliens[(row * max_columns) + col];
            spr->modifyPosition((col * spr->getSize().x) + (col * paddingX), (row * spr->getSize().y) + (row * paddingY));
        }
    }
}



void StateGameplay::onStateLeave()
{
}



void StateGameplay::tick(float dt)
{
    if (m_player_shooting && !m_player_projectile)
    {
        m_player_projectile = getObjectFactory().createSprite(
            "..\\..\\Resources\\Textures\\projectile.png",
            { m_player->getPosition().x + (m_player->getSize().x / 2), 
              m_player->getPosition().y - 5 });
    }

    if (m_player_projectile)
    {
        m_player_projectile->modifyPosition(0, -m_player_projectile_speed * dt);

        // Collision test with alien block.
        for (auto& alien : m_aliens)
        {
            if (alien)
            {
                if (m_player_projectile->collisionTest(alien))
                {
                    alien = nullptr;
                    m_player_projectile = nullptr;
                    break;
                }
            }
        }

        // If projectile hasn't hit anything, delete it at the top of the screen.
        if (m_player_projectile && m_player_projectile->getPosition().y <= WINDOW_MARGIN)
        {
            m_player_projectile = nullptr;
        }
    }

    if (m_player_direction == MoveDirection::LEFT)
    {
        if (m_player->getPosition().x > WINDOW_LEFT_BOUNDARY)
        {
            m_player->modifyPosition(-m_player_speed * dt, 0);
        }
    }

    if (m_player_direction == MoveDirection::RIGHT)
    {
        if (m_player->getPosition().x + m_player->getSize().x < WINDOW_RIGHT_BOUNDARY)
        {
            m_player->modifyPosition(m_player_speed * dt, 0);
        }
    }
}



void StateGameplay::onCommand(const Command c, const CommandState s)
{
    if (c == Command::MOVE_LEFT)
    {
        if (s == CommandState::PRESSED || s == CommandState::REPEATING)
        {
            m_player_direction = MoveDirection::LEFT;
        }
        else
        {
            m_player_direction = MoveDirection::NONE;
        }
    }

    if (c == Command::MOVE_RIGHT)
    {
        if (s == CommandState::PRESSED || s == CommandState::REPEATING)
        {
            m_player_direction = MoveDirection::RIGHT;
        }
        else
        {
            m_player_direction = MoveDirection::NONE;
        }
    }

    if (c == Command::SHOOT)
    {
        if (s == CommandState::PRESSED || s == CommandState::REPEATING)
        {
            m_player_shooting = true;
        }
        else
        {
            m_player_shooting = false;
        }
    }

    if (c == Command::PAUSE)
    {
        if (s == CommandState::PRESSED)
        {
            // pause
        }
    }

    std::cout << "command: " << static_cast<int>(c) << " " << static_cast<int>(s) << std::endl;
}


