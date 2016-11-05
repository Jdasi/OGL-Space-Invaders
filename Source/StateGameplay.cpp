#include <iostream>

#include "StateGameplay.h"
#include "Game.h"
#include "Constants.h"

StateGameplay::StateGameplay(ObjectFactory& factory)
    : State(factory)
    , m_player_start({ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 })
    , m_player_speed(WINDOW_WIDTH / 100)
    , m_player_shooting(false)
    , m_player_direction(MoveDirection::NONE)
{
}



StateGameplay::~StateGameplay()
{
}



void StateGameplay::onStateEnter()
{
    m_player = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\Player.png", m_player_start);
    m_text = getObjectFactory().createText("Test", { 375, 325 }, 1.0f, ASGE::COLOURS::DARKORANGE);

    row1.push_back(getObjectFactory().createSprite("..\\..\\Resources\\Textures\\Player.png", { 300, 300 }));
}



void StateGameplay::onStateLeave()
{
}



void StateGameplay::tick()
{
    if (m_player_shooting && !m_player_projectile)
    {
        m_player_projectile = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\Player.png", m_player->getPosition());
    }

    if (m_player_projectile)
    {
        m_player_projectile->modifyPosition(0, -5);

        if (m_player_projectile->getPosition().y <= 0)
        {
            m_player_projectile = nullptr;
        }
    }

    if (m_player_direction == MoveDirection::LEFT)
    {
        if (m_player->getPosition().x > WINDOW_LEFT_BOUNDARY)
        {
            m_player->modifyPosition(-m_player_speed, 0);
        }
    }

    if (m_player_direction == MoveDirection::RIGHT)
    {
        if (m_player->getPosition().x + m_player->getSize().x < WINDOW_RIGHT_BOUNDARY)
        {
            m_player->modifyPosition(m_player_speed, 0);
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


