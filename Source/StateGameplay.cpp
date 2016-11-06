#include <iostream>

#include "StateGameplay.h"
#include "Game.h"
#include "Constants.h"
#include "ObjectBlock.h"

StateGameplay::StateGameplay(ObjectFactory& factory)
    : State(factory)
    , m_player_start({ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 })
    , m_player_speed(400)
    , m_player_projectile_speed(600)
    , m_player_shooting(false)
    , m_player_direction(MoveDirection::NONE)
    , m_aliens(nullptr)
{
}



StateGameplay::~StateGameplay()
{
}



void StateGameplay::onStateEnter()
{
    m_player = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\player.png", m_player_start);
    m_score_text = getObjectFactory().createText("Score:", { 20, 30 }, 0.7f, ASGE::COLOURS::DARKORANGE);

    int max_columns = 11;
    int padding_x = 10;
    int padding_y = 20;
    Vector2 alien_start{ 100, 100 };

    m_aliens = std::make_unique<ObjectBlock>(alien_start, max_columns, padding_x, padding_y);

    std::string alien_img = "..\\..\\Resources\\Textures\\top_alien_0.png";
    for (int i = 0; i < max_columns; ++i)
    {
        m_aliens->addObject(getObjectFactory().createSprite(alien_img, alien_start));
    }

    int double_row = max_columns * 2;
    alien_img = "..\\..\\Resources\\Textures\\middle_alien_0.png";
    for (int i = 0; i < double_row; ++i)
    {
        m_aliens->addObject(getObjectFactory().createSprite(alien_img, alien_start));
    }

    alien_img = "..\\..\\Resources\\Textures\\bottom_alien_0.png";
    for (int i = 0; i < double_row; ++i)
    {
        m_aliens->addObject(getObjectFactory().createSprite(alien_img, alien_start));
    }

    m_aliens->updateLayout();
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

        // Destroy the projectile if it collides with something or hits the top edge.
        if (m_aliens->collisionTest(m_player_projectile) ||
            m_player_projectile->getPosition().y <= WINDOW_MARGIN)
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


