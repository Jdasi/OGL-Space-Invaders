#include <iostream>

#include "StateGameplay.h"
#include "Game.h"
#include "Constants.h"

StateGameplay::StateGameplay(ObjectFactory& factory)
    : State(factory)
    , m_player_start({ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 })
    , m_player_speed(500)
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
    m_text = getObjectFactory().createText("Test", { 375, 325 }, 1.0f, ASGE::COLOURS::DARKORANGE);

    int max_rows = 5;
    int max_columns = 11;
    int padding = 20;
    Vector2 alien_start{ 100, 100 };

    for (int row = 0; row < max_rows; ++row)
    {
        for (int col = 0; col < max_columns; ++col)
        {
            m_aliens.push_back(getObjectFactory().createSprite("..\\..\\Resources\\Textures\\player.png", alien_start));

            auto spr = m_aliens[(row * max_columns) + col];
            spr->modifyPosition((col * spr->getSize().x) + (col * padding), (row * spr->getSize().y) + (row * padding));
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
        m_player_projectile->modifyPosition(0, -m_player_speed * dt);

        if (m_player_projectile->getPosition().y <= WINDOW_MARGIN)
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

    std::cout << "command: " << static_cast<int>(c) << " " << static_cast<int>(s) << " " << m_player->getPosition().x << ", " << m_player->getPosition().y << std::endl;
}


